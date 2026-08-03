#include "../../include/cub3d.h"

/*
** 3x5 bitmap glyphs, 15 bits row-major from the top-left: digits 0-9,
** then ':' and '.'. Enough for a timer and a speedometer with no font
** dependency at all.
*/
static const uint16_t	g_font[12] = {
	0x7B6F, 0x2C97, 0x73E7, 0x73CF, 0x5BC9, 0x79CF,
	0x79EF, 0x7292, 0x7BEF, 0x7BCF, 0x0410, 0x0002
};

static void	draw_glyph(t_fb *fb, int x, int y, int scale, uint16_t bits,
			uint32_t color)
{
	int	row;
	int	col;
	int	i;
	int	j;

	row = -1;
	while (++row < 5)
	{
		col = -1;
		while (++col < 3)
		{
			if (!((bits >> (14 - (row * 3 + col))) & 1))
				continue ;
			i = -1;
			while (++i < scale)
			{
				j = -1;
				while (++j < scale)
					fb_put_pixel(fb, x + col * scale + j,
						y + row * scale + i, color);
			}
		}
	}
}

static void	draw_text(t_fb *fb, int x, int y, int scale, const char *str,
			uint32_t color)
{
	int	idx;

	while (*str)
	{
		idx = -1;
		if (*str >= '0' && *str <= '9')
			idx = *str - '0';
		else if (*str == ':')
			idx = 10;
		else if (*str == '.')
			idx = 11;
		if (idx >= 0)
			draw_glyph(fb, x, y, scale, g_font[idx], color);
		x += 4 * scale;
		str++;
	}
}

static void	fmt_time(char *buf, double t)
{
	int	cs;

	cs = (int)(t * 100.0);
	buf[0] = '0' + (cs / 6000) % 10;
	buf[1] = ':';
	buf[2] = '0' + (cs / 1000) % 6;
	buf[3] = '0' + (cs / 100) % 10;
	buf[4] = '.';
	buf[5] = '0' + (cs / 10) % 10;
	buf[6] = '0' + cs % 10;
	buf[7] = '\0';
}

static uint32_t	bar_color(double ratio)
{
	int	r;
	int	g;
	int	b;

	r = (int)(0x4C + ratio * (0xFF - 0x4C));
	g = (int)(0xA6 + ratio * (0x4C - 0xA6));
	b = (int)(0xFF + ratio * (0x3C - 0xFF));
	return (0xFF000000u | (r << 16) | (g << 8) | b);
}

static void	draw_speed_bar(t_game *game, double speed)
{
	const int	w = 360;
	const int	h = 8;
	int			x0;
	int			y0;
	int			x;
	int			y;
	int			fill;
	double		ratio;

	ratio = speed / SPEED_CAP;
	if (ratio > 1.0)
		ratio = 1.0;
	fill = (int)(w * ratio);
	x0 = (game->fb.width - w) / 2;
	y0 = game->fb.height - 48;
	y = -1;
	while (++y < h)
	{
		x = -1;
		while (++x < w)
		{
			if (x < fill)
				fb_put_pixel(&game->fb, x0 + x, y0 + y, bar_color(ratio));
			else
				fb_put_pixel(&game->fb, x0 + x, y0 + y, 0xFF20242C);
		}
	}
}

static void	draw_timer(t_game *game)
{
	char		buf[8];
	uint32_t	color;
	int			x;

	color = 0xFF9AA0AA;
	if (game->race_state == RACE_RUNNING)
		color = 0xFFF2F2F2;
	else if (game->race_state == RACE_FINISHED)
		color = 0xFF57E389;
	fmt_time(buf, game->run_time);
	x = (game->fb.width - (7 * 4 * 6 - 6)) / 2;
	draw_text(&game->fb, x, 24, 6, buf, color);
	if (game->best_time >= 0)
	{
		fmt_time(buf, game->best_time);
		x = (game->fb.width - (7 * 4 * 3 - 3)) / 2;
		draw_text(&game->fb, x, 64, 3, buf, 0xFFC8B36A);
	}
}

void	render_hud(t_game *game)
{
	char	buf[8];
	double	speed;
	int		v;
	int		i;

	speed = sqrt(game->player.vx * game->player.vx
			+ game->player.vy * game->player.vy
			+ game->player.vz * game->player.vz);
	draw_speed_bar(game, speed);
	v = (int)(speed * 10.0);
	i = 0;
	if (v >= 100)
		buf[i++] = '0' + v / 100;
	buf[i++] = '0' + (v / 10) % 10;
	buf[i++] = '.';
	buf[i++] = '0' + v % 10;
	buf[i] = '\0';
	draw_text(&game->fb, (game->fb.width - 360) / 2,
		game->fb.height - 76, 4, buf, 0xFFF2F2F2);
	if (game->has_goal)
		draw_timer(game);
}
