#include "../../include/cub3d.h"

static uint32_t	lerp_color(uint32_t a, uint32_t b, double f)
{
	int	r;
	int	g;
	int	bl;

	if (f < 0)
		f = 0;
	if (f > 1)
		f = 1;
	r = (int)(((a >> 16) & 0xFF) * (1 - f) + ((b >> 16) & 0xFF) * f);
	g = (int)(((a >> 8) & 0xFF) * (1 - f) + ((b >> 8) & 0xFF) * f);
	bl = (int)((a & 0xFF) * (1 - f) + (b & 0xFF) * f);
	return (0xFF000000u | (r << 16) | (g << 8) | bl);
}

static uint32_t	band_color(double h)
{
	if (h < 1.5)
		return (lerp_color(0xFF39555E, 0xFF4E7A50, h / 1.5));
	if (h < 4.0)
		return (lerp_color(0xFF4E7A50, 0xFF6E6A58, (h - 1.5) / 2.5));
	if (h < 6.5)
		return (lerp_color(0xFF6E6A58, 0xFF8A8884, (h - 4.0) / 2.5));
	return (lerp_color(0xFF8A8884, 0xFFE8ECF0, (h - 6.5) / 2.5));
}

/*
** Height-banded palette, lambert shading from the analytic gradient,
** cell grid lines for speed perception, checkered goal pads, and
** distance fog into the sky color.
*/
static uint32_t	ground_color(t_game *game, double x, double y, double t)
{
	double		h;
	double		g[2];
	double		shade;
	uint32_t	color;
	int			cx;

	h = terrain_h(&game->map, x, y);
	cx = (int)x;
	if (cx >= 0 && cx < game->map.width && (int)y >= 0
		&& (int)y < game->map.height
		&& game->map.grid[(int)y][cx] == 'G')
		color = 0xFFE0323C;
	else
		color = band_color(h);
	if (x - floor(x) < 0.06 || y - floor(y) < 0.06)
		color = lerp_color(color, 0xFF10141E, 0.35);
	terrain_grad(&game->map, x, y, g);
	shade = (0.30 * g[0] - 0.42 * g[1] + 0.85)
		/ sqrt(g[0] * g[0] + g[1] * g[1] + 1.0);
	if (shade < 0.25)
		shade = 0.25;
	if (shade > 1.0)
		shade = 1.0;
	color = lerp_color(0xFF000000, color, shade);
	return (lerp_color(color, 0xFF39415A, t / TERRAIN_DIST));
}

static void	sky_column(t_game *game, int x, int top, int horizon)
{
	int			y;
	double		f;
	uint32_t	color;

	y = -1;
	while (++y < top)
	{
		f = (horizon - y) / (game->fb.height * 0.5 + 1.0);
		color = lerp_color(0xFF39415A, 0xFF10141E, f);
		fb_put_pixel(&game->fb, x, y, color);
	}
}

static void	terrain_column(t_game *game, int x, double eye_z, int horizon)
{
	double	camera_x;
	double	rx;
	double	ry;
	double	t;
	int		ybuf;
	int		proj;
	int		y;

	camera_x = (2.0 * x / game->fb.width - 1.0) * game->fov_scale;
	rx = game->player.dir_x + game->player.plane_x * camera_x;
	ry = game->player.dir_y + game->player.plane_y * camera_x;
	ybuf = game->fb.height;
	t = 0.35;
	while (t < TERRAIN_DIST && ybuf > 0)
	{
		proj = horizon + (int)((eye_z
					- terrain_h(&game->map, game->player.x + rx * t,
						game->player.y + ry * t)) / t * game->fb.height);
		if (proj < ybuf)
		{
			y = proj - 1;
			while (++y < ybuf)
				if (y >= 0)
					fb_put_pixel(&game->fb, x, y, ground_color(game,
							game->player.x + rx * t,
							game->player.y + ry * t, t));
			ybuf = proj;
		}
		t += 0.012 * t + 0.008;
	}
	sky_column(game, x, ybuf, horizon);
}

void	render_terrain(t_game *game)
{
	double	eye_z;
	int		horizon;
	int		x;

	eye_z = game->player.z + EYE_H;
	horizon = game->fb.height / 2
		+ (int)(game->player.pitch * game->fb.height);
	x = -1;
	while (++x < game->fb.width)
		terrain_column(game, x, eye_z, horizon);
}
