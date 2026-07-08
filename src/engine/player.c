#include "../../include/cub3d.h"

bool	is_walkable(t_map *map, double x, double y)
{
	int	cx;
	int	cy;

	cx = (int)x;
	cy = (int)y;
	if (x < 0 || y < 0 || cx >= map->width || cy >= map->height)
		return (false);
	return (map->grid[cy][cx] == '0');
}

static bool	can_stand(t_map *map, double x, double y)
{
	const double	r = COLLISION_RADIUS;

	return (is_walkable(map, x - r, y - r) && is_walkable(map, x + r, y - r)
		&& is_walkable(map, x - r, y + r) && is_walkable(map, x + r, y + r));
}

/*
** Moves one axis at a time so the player slides along walls instead of
** sticking to them. The collision box is a square of COLLISION_RADIUS.
*/
static void	try_move(t_game *game, double dx, double dy)
{
	t_player	*p;

	p = &game->player;
	if (can_stand(&game->map, p->x + dx, p->y))
		p->x += dx;
	if (can_stand(&game->map, p->x, p->y + dy))
		p->y += dy;
}

static void	rotate(t_player *p, double angle)
{
	double	old_dir_x;
	double	old_plane_x;

	old_dir_x = p->dir_x;
	p->dir_x = p->dir_x * cos(angle) - p->dir_y * sin(angle);
	p->dir_y = old_dir_x * sin(angle) + p->dir_y * cos(angle);
	old_plane_x = p->plane_x;
	p->plane_x = p->plane_x * cos(angle) - p->plane_y * sin(angle);
	p->plane_y = old_plane_x * sin(angle) + p->plane_y * cos(angle);
}

void	move_player(t_game *game, double dt)
{
	t_player	*p;
	double		step;
	double		dx;
	double		dy;

	p = &game->player;
	step = MOVE_SPEED * dt;
	dx = 0;
	dy = 0;
	if (game->keys.forward)
	{
		dx += p->dir_x * step;
		dy += p->dir_y * step;
	}
	if (game->keys.back)
	{
		dx -= p->dir_x * step;
		dy -= p->dir_y * step;
	}
	if (game->keys.strafe_l)
	{
		dx += p->dir_y * step;
		dy -= p->dir_x * step;
	}
	if (game->keys.strafe_r)
	{
		dx -= p->dir_y * step;
		dy += p->dir_x * step;
	}
	if (dx != 0 || dy != 0)
		try_move(game, dx, dy);
	if (game->keys.turn_l)
		rotate(p, -ROT_SPEED * dt);
	if (game->keys.turn_r)
		rotate(p, ROT_SPEED * dt);
}
