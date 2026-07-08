#include "../../include/cub3d.h"

#define MM_MARGIN 16

static uint32_t	blend50(uint32_t a, uint32_t b)
{
	return (0xFF000000u | (((a >> 1) & 0x7F7F7F) + ((b >> 1) & 0x7F7F7F)));
}

static int	tile_size(t_game *game)
{
	int	tile_w;
	int	tile_h;
	int	tile;

	tile_w = game->fb.width / 4 / game->map.width;
	tile_h = game->fb.height / 4 / game->map.height;
	tile = tile_w;
	if (tile_h < tile)
		tile = tile_h;
	if (tile < 2)
		tile = 2;
	if (tile > 12)
		tile = 12;
	return (tile);
}

static void	draw_cell(t_game *game, int cx, int cy, int tile)
{
	uint32_t	color;
	int			x;
	int			y;

	if (game->map.grid[cy][cx] == ' ')
		return ;
	color = 0xFF20242C;
	if (game->map.grid[cy][cx] == '1')
		color = 0xFFAAB4C8;
	y = -1;
	while (++y < tile - 1)
	{
		x = -1;
		while (++x < tile - 1)
			fb_put_pixel(&game->fb, MM_MARGIN + cx * tile + x,
				MM_MARGIN + cy * tile + y, blend50(color,
					game->fb.pixels[(MM_MARGIN + cy * tile + y)
					* game->fb.width + MM_MARGIN + cx * tile + x]));
	}
}

typedef struct s_point
{
	int	x;
	int	y;
}	t_point;

static void	draw_line(t_fb *fb, t_point a, t_point b, uint32_t color)
{
	t_point	delta;
	t_point	step;
	int		err;
	int		e2;

	delta.x = abs(b.x - a.x);
	delta.y = -abs(b.y - a.y);
	step.x = 1 - 2 * (a.x > b.x);
	step.y = 1 - 2 * (a.y > b.y);
	err = delta.x + delta.y;
	while (1)
	{
		fb_put_pixel(fb, a.x, a.y, color);
		if (a.x == b.x && a.y == b.y)
			break ;
		e2 = 2 * err;
		if (e2 >= delta.y)
		{
			err += delta.y;
			a.x += step.x;
		}
		if (e2 <= delta.x)
		{
			err += delta.x;
			a.y += step.y;
		}
	}
}

static void	draw_player(t_game *game, int tile)
{
	t_point	pos;
	t_point	tip;
	int		x;
	int		y;

	pos.x = MM_MARGIN + (int)(game->player.x * tile);
	pos.y = MM_MARGIN + (int)(game->player.y * tile);
	tip.x = pos.x + (int)(game->player.dir_x * tile * 1.5);
	tip.y = pos.y + (int)(game->player.dir_y * tile * 1.5);
	draw_line(&game->fb, pos, tip, 0xFFFFD24C);
	y = -3;
	while (++y < 3)
	{
		x = -3;
		while (++x < 3)
			if (x * x + y * y <= 4)
				fb_put_pixel(&game->fb, pos.x + x, pos.y + y, 0xFFE84C4C);
	}
}

void	render_minimap(t_game *game)
{
	int	tile;
	int	x;
	int	y;

	tile = tile_size(game);
	y = 0;
	while (y < game->map.height)
	{
		x = 0;
		while (x < game->map.width)
		{
			draw_cell(game, x, y, tile);
			x++;
		}
		y++;
	}
	draw_player(game, tile);
}
