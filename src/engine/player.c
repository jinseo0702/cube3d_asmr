#include "../../include/cub3d.h"

bool	is_walkable(t_map *map, double x, double y)
{
	int	cx;
	int	cy;

	cx = (int)x;
	cy = (int)y;
	if (x < 0 || y < 0 || cx >= map->width || cy >= map->height)
		return (false);
	return (map->grid[cy][cx] == '0' || map->grid[cy][cx] == 'G');
}

static bool	can_stand(t_map *map, double x, double y)
{
	const double	r = COLLISION_RADIUS;

	return (is_walkable(map, x - r, y - r) && is_walkable(map, x + r, y - r)
		&& is_walkable(map, x - r, y + r) && is_walkable(map, x + r, y + r));
}

/*
** Axis-separated collision: a blocked axis kills only that velocity
** component, so grazing a wall at a shallow angle keeps almost all of
** your speed — this slide is the 2D equivalent of riding a surf ramp.
*/
static void	slide_move(t_game *game, double dt)
{
	t_player	*p;

	p = &game->player;
	if (can_stand(&game->map, p->x + p->vx * dt, p->y))
		p->x += p->vx * dt;
	else
		p->vx = 0;
	if (can_stand(&game->map, p->x, p->y + p->vy * dt))
		p->y += p->vy * dt;
	else
		p->vy = 0;
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

bool	wish_dir(t_game *game, double *wx, double *wy)
{
	t_player	*p;
	double		len;

	p = &game->player;
	*wx = 0;
	*wy = 0;
	if (game->keys.forward)
	{
		*wx += p->dir_x;
		*wy += p->dir_y;
	}
	if (game->keys.back)
	{
		*wx -= p->dir_x;
		*wy -= p->dir_y;
	}
	if (game->keys.strafe_l)
	{
		*wx += p->dir_y;
		*wy -= p->dir_x;
	}
	if (game->keys.strafe_r)
	{
		*wx -= p->dir_y;
		*wy += p->dir_x;
	}
	len = sqrt(*wx * *wx + *wy * *wy);
	if (len < 1e-9)
		return (false);
	*wx /= len;
	*wy /= len;
	return (true);
}

/*
** Quake's accelerate: only the velocity component already along wishdir
** counts against the cap, so pushing perpendicular to your velocity
** keeps adding speed. Air-strafing falls out of these ten lines.
*/
void	accelerate(t_player *p, double wx, double wy, double dt)
{
	double	add;
	double	accel;
	double	speed;

	add = WISH_SPEED - (p->vx * wx + p->vy * wy);
	if (add <= 0)
		return ;
	accel = ACCEL * WISH_SPEED * dt;
	if (accel > add)
		accel = add;
	p->vx += accel * wx;
	p->vy += accel * wy;
	speed = sqrt(p->vx * p->vx + p->vy * p->vy);
	if (speed > SPEED_CAP)
	{
		p->vx *= SPEED_CAP / speed;
		p->vy *= SPEED_CAP / speed;
	}
}

/*
** Shared by both movement modes: arrow-key and mouse yaw, and mouse
** pitch (a screen-space horizon shift, only rendered in terrain mode).
*/
void	apply_turn(t_game *game, double dt)
{
	t_player	*p;

	p = &game->player;
	if (game->keys.turn_l)
		rotate(p, -ROT_SPEED * dt);
	if (game->keys.turn_r)
		rotate(p, ROT_SPEED * dt);
	if (game->keys.turn != 0)
	{
		rotate(p, game->keys.turn);
		game->keys.turn = 0;
	}
	p->pitch += game->keys.look;
	game->keys.look = 0;
	if (p->pitch > PITCH_MAX)
		p->pitch = PITCH_MAX;
	if (p->pitch < -PITCH_MAX)
		p->pitch = -PITCH_MAX;
}

void	move_player(t_game *game, double dt)
{
	t_player	*p;
	double		wx;
	double		wy;
	double		damp;

	p = &game->player;
	apply_turn(game, dt);
	if (wish_dir(game, &wx, &wy))
		accelerate(p, wx, wy, dt);
	else
	{
		damp = 1.0 - FRICTION * dt;
		if (damp < 0)
			damp = 0;
		p->vx *= damp;
		p->vy *= damp;
	}
	slide_move(game, dt);
}
