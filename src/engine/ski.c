#include "../../include/cub3d.h"

static double	cell_h(t_map *map, int cx, int cy)
{
	if (cx < 0)
		cx = 0;
	if (cy < 0)
		cy = 0;
	if (cx >= map->width)
		cx = map->width - 1;
	if (cy >= map->height)
		cy = map->height - 1;
	return (map->hmap[cy * map->width + cx]);
}

static double	digit_h(t_map *map, int cx, int cy)
{
	char	c;

	if (cx < 0)
		cx = 0;
	if (cy < 0)
		cy = 0;
	if (cx >= map->width)
		cx = map->width - 1;
	if (cy >= map->height)
		cy = map->height - 1;
	c = map->grid[cy][cx];
	if (c >= '0' && c <= '9')
		return (c - '0');
	return (0);
}

/*
** Rounding heights to digits leaves staircase terraces in the data
** (8,8,7,7,6...), which read as endless micro-crests that eject the
** rider. One 3x3 blur pass turns them back into continuous slopes.
*/
int	terrain_init(t_map *map)
{
	int		x;
	int		y;
	int		i;
	int		j;
	double	acc;

	map->hmap = ft_calloc(map->width * map->height, sizeof(double));
	if (!map->hmap)
		return (error_msg("allocation failure"));
	y = -1;
	while (++y < map->height)
	{
		x = -1;
		while (++x < map->width)
		{
			acc = 0;
			i = -2;
			while (++i < 2)
			{
				j = -2;
				while (++j < 2)
					acc += digit_h(map, x + j, y + i)
						* (2 - (i != 0)) * (2 - (j != 0));
			}
			map->hmap[y * map->width + x] = acc / 16.0;
		}
	}
	return (0);
}

/*
** Height between cell centers with smoothstep-weighted interpolation:
** unlike plain bilinear (C0, slope kinks at every cell edge that eject
** the rider and bleed speed), this surface has continuous slope, so
** you actually ride it.
*/
double	terrain_h(t_map *map, double x, double y)
{
	int		gx;
	int		gy;
	double	tx;
	double	ty;
	double	a;
	double	b;

	x -= 0.5;
	y -= 0.5;
	gx = (int)floor(x);
	gy = (int)floor(y);
	tx = x - gx;
	ty = y - gy;
	tx = tx * tx * (3.0 - 2.0 * tx);
	ty = ty * ty * (3.0 - 2.0 * ty);
	a = cell_h(map, gx, gy) * (1 - tx) + cell_h(map, gx + 1, gy) * tx;
	b = cell_h(map, gx, gy + 1) * (1 - tx)
		+ cell_h(map, gx + 1, gy + 1) * tx;
	return (a * (1 - ty) + b * ty);
}

void	terrain_grad(t_map *map, double x, double y, double *g)
{
	const double	e = 0.05;

	g[0] = (terrain_h(map, x + e, y) - terrain_h(map, x - e, y)) / (2 * e);
	g[1] = (terrain_h(map, x, y + e) - terrain_h(map, x, y - e)) / (2 * e);
}

/*
** The surf core: clip velocity against the surface normal. Landing on a
** slope keeps the tangential component (momentum-preserving landings),
** and gravity clipped every tick becomes downhill acceleration.
*/
static void	ground_clip(t_game *game, t_player *p)
{
	double	g[2];
	double	n[3];
	double	len;
	double	d;

	terrain_grad(&game->map, p->x, p->y, g);
	len = sqrt(g[0] * g[0] + g[1] * g[1] + 1.0);
	n[0] = -g[0] / len;
	n[1] = -g[1] / len;
	n[2] = 1.0 / len;
	d = p->vx * n[0] + p->vy * n[1] + p->vz * n[2];
	if (d < 0)
	{
		p->vx -= d * n[0];
		p->vy -= d * n[1];
		p->vz -= d * n[2];
	}
}

static void	fence(t_map *map, t_player *p)
{
	if (p->x < 0.6)
		p->x = 0.6;
	if (p->y < 0.6)
		p->y = 0.6;
	if (p->x > map->width - 0.6)
		p->x = map->width - 0.6;
	if (p->y > map->height - 0.6)
		p->y = map->height - 0.6;
}

static void	cap_speed(t_player *p)
{
	double	speed;

	speed = sqrt(p->vx * p->vx + p->vy * p->vy + p->vz * p->vz);
	if (speed > SKI_SPEED_MAX)
	{
		p->vx *= SKI_SPEED_MAX / speed;
		p->vy *= SKI_SPEED_MAX / speed;
		p->vz *= SKI_SPEED_MAX / speed;
	}
}

/*
** Grounded motion is constrained to the surface: gravity projected onto
** the slope (-g grad / (1 + |grad|^2)) accelerates you downhill and the
** vertical speed is whatever following the ground requires. You leave
** the ground only when it curves away faster than gravity can pull you
** down (crest launches) — or by jumping.
*/
static void	ground_tick(t_game *game, t_player *p, double dt)
{
	double	g[2];
	double	f;
	double	want;

	terrain_grad(&game->map, p->x, p->y, g);
	f = GRAVITY / (1.0 + g[0] * g[0] + g[1] * g[1]);
	p->vx -= f * g[0] * dt;
	p->vy -= f * g[1] * dt;
	p->x += p->vx * dt;
	p->y += p->vy * dt;
	fence(&game->map, p);
	want = (terrain_h(&game->map, p->x, p->y) - p->z) / dt;
	if (want < p->vz - GRAVITY * dt * 8.0)
	{
		p->on_ground = false;
		p->z += p->vz * dt;
	}
	else
	{
		p->vz = want;
		p->z = terrain_h(&game->map, p->x, p->y);
	}
}

static void	air_tick(t_game *game, t_player *p, double dt)
{
	double	h;

	p->vz -= GRAVITY * dt;
	p->x += p->vx * dt;
	p->y += p->vy * dt;
	p->z += p->vz * dt;
	fence(&game->map, p);
	h = terrain_h(&game->map, p->x, p->y);
	if (p->z <= h)
	{
		p->z = h;
		ground_clip(game, p);
		p->on_ground = true;
	}
}

void	ski_move(t_game *game, double dt)
{
	t_player	*p;
	double		wx;
	double		wy;

	p = &game->player;
	apply_turn(game, dt);
	if (wish_dir(game, &wx, &wy))
		accelerate(p, wx, wy, dt);
	else if (p->on_ground)
	{
		p->vx *= 1.0 - SKI_FRICTION * dt;
		p->vy *= 1.0 - SKI_FRICTION * dt;
	}
	if (game->keys.jump && p->on_ground)
	{
		p->vz += JUMP_VZ;
		p->on_ground = false;
	}
	if (p->on_ground)
		ground_tick(game, p, dt);
	else
		air_tick(game, p, dt);
	cap_speed(p);
}
