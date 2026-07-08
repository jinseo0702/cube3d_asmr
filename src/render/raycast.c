#include "../../include/cub3d.h"

static void	init_ray(t_game *game, t_ray *ray, int x)
{
	double	camera_x;

	camera_x = 2.0 * x / game->fb.width - 1.0;
	ray->dir_x = game->player.dir_x + game->player.plane_x * camera_x;
	ray->dir_y = game->player.dir_y + game->player.plane_y * camera_x;
	ray->map_x = (int)game->player.x;
	ray->map_y = (int)game->player.y;
	ray->delta_x = 1e30;
	ray->delta_y = 1e30;
	if (ray->dir_x != 0)
		ray->delta_x = fabs(1.0 / ray->dir_x);
	if (ray->dir_y != 0)
		ray->delta_y = fabs(1.0 / ray->dir_y);
	ray->step_x = 1;
	ray->step_y = 1;
	ray->side_dist_x = (ray->map_x + 1.0 - game->player.x) * ray->delta_x;
	ray->side_dist_y = (ray->map_y + 1.0 - game->player.y) * ray->delta_y;
	if (ray->dir_x < 0)
	{
		ray->step_x = -1;
		ray->side_dist_x = (game->player.x - ray->map_x) * ray->delta_x;
	}
	if (ray->dir_y < 0)
	{
		ray->step_y = -1;
		ray->side_dist_y = (game->player.y - ray->map_y) * ray->delta_y;
	}
}

/*
** Classic DDA: advance the ray one grid boundary at a time (whichever axis
** boundary is closer) until a wall cell is hit. The perpendicular distance
** avoids the fisheye distortion a euclidean distance would cause.
*/
static void	cast_ray(t_game *game, t_ray *ray)
{
	char	cell;

	while (1)
	{
		if (ray->side_dist_x < ray->side_dist_y)
		{
			ray->side_dist_x += ray->delta_x;
			ray->map_x += ray->step_x;
			ray->side = 0;
		}
		else
		{
			ray->side_dist_y += ray->delta_y;
			ray->map_y += ray->step_y;
			ray->side = 1;
		}
		if (ray->map_x < 0 || ray->map_x >= game->map.width
			|| ray->map_y < 0 || ray->map_y >= game->map.height)
			break ;
		cell = game->map.grid[ray->map_y][ray->map_x];
		if (cell == '1' || cell == ' ')
			break ;
	}
	if (ray->side == 0)
		ray->dist = ray->side_dist_x - ray->delta_x;
	else
		ray->dist = ray->side_dist_y - ray->delta_y;
	if (ray->dist < 1e-6)
		ray->dist = 1e-6;
}

static t_tex	*pick_texture(t_game *game, t_ray *ray)
{
	if (ray->side == 0 && ray->dir_x > 0)
		return (&game->tex[EAST]);
	if (ray->side == 0)
		return (&game->tex[WEST]);
	if (ray->dir_y > 0)
		return (&game->tex[SOUTH]);
	return (&game->tex[NORTH]);
}

static int	wall_tex_x(t_game *game, t_ray *ray, t_tex *tex)
{
	double	wall_x;
	int		tex_x;

	if (ray->side == 0)
		wall_x = game->player.y + ray->dist * ray->dir_y;
	else
		wall_x = game->player.x + ray->dist * ray->dir_x;
	wall_x -= floor(wall_x);
	tex_x = (int)(wall_x * tex->width);
	if ((ray->side == 0 && ray->dir_x > 0)
		|| (ray->side == 1 && ray->dir_y < 0))
		tex_x = tex->width - tex_x - 1;
	return (tex_x);
}

static void	draw_column(t_game *game, t_ray *ray, int x)
{
	t_tex		*tex;
	uint32_t	color;
	int			line_h;
	int			top;
	int			tex_x;
	int			y;

	tex = pick_texture(game, ray);
	tex_x = wall_tex_x(game, ray, tex);
	line_h = (int)(game->fb.height / ray->dist);
	top = game->fb.height / 2 - line_h / 2;
	y = top - 1;
	while (++y < top + line_h)
	{
		if (y < 0 || y >= game->fb.height)
			continue ;
		color = tex->pixels[(int)((double)(y - top) / line_h * tex->height)
			* tex->width + tex_x];
		if (ray->side == 1)
			color = ((color >> 1) & 0x7F7F7F) | 0xFF000000;
		fb_put_pixel(&game->fb, x, y, color);
	}
}

void	render_frame(t_game *game)
{
	t_ray	ray;
	int		x;
	int		y;

	y = -1;
	while (++y < game->fb.height / 2)
	{
		x = -1;
		while (++x < game->fb.width)
			fb_put_pixel(&game->fb, x, y, game->map.ceil_color);
	}
	while (y < game->fb.height)
	{
		x = -1;
		while (++x < game->fb.width)
			fb_put_pixel(&game->fb, x, y, game->map.floor_color);
		y++;
	}
	x = -1;
	while (++x < game->fb.width)
	{
		init_ray(game, &ray, x);
		cast_ray(game, &ray);
		draw_column(game, &ray, x);
	}
	if (game->show_minimap)
		render_minimap(game);
}
