#include "../../include/cub3d.h"

static void	set_direction(t_player *p, char dir)
{
	p->dir_x = 0;
	p->dir_y = 0;
	p->plane_x = 0;
	p->plane_y = 0;
	if (dir == 'N')
	{
		p->dir_y = -1;
		p->plane_x = FOV;
	}
	else if (dir == 'S')
	{
		p->dir_y = 1;
		p->plane_x = -FOV;
	}
	else if (dir == 'E')
	{
		p->dir_x = 1;
		p->plane_y = FOV;
	}
	else if (dir == 'W')
	{
		p->dir_x = -1;
		p->plane_y = -FOV;
	}
}

static void	race_reset(t_game *game)
{
	game->player.x = game->map.spawn_x;
	game->player.y = game->map.spawn_y;
	game->player.vx = 0;
	game->player.vy = 0;
	game->player.vz = 0;
	game->player.z = 0;
	game->player.pitch = 0;
	game->player.on_ground = true;
	if (game->map.terrain)
		game->player.z = terrain_h(&game->map, game->player.x,
				game->player.y);
	set_direction(&game->player, game->map.spawn_dir);
	game->race_state = RACE_IDLE;
	game->run_time = 0;
	game->fov_scale = 1.0;
}

/*
** register_player() blanks the spawn cell to '0', which in terrain mode
** would dig a pit under the player. Refill it with its tallest neighbor.
*/
static void	patch_spawn_height(t_map *map)
{
	int		x;
	int		y;
	int		i;
	char	best;
	char	c;

	if (!map->terrain)
		return ;
	x = (int)map->spawn_x;
	y = (int)map->spawn_y;
	best = '0';
	i = -1;
	while (++i < 4)
	{
		c = map->grid[y + (i == 1) - (i == 0)][x + (i == 3) - (i == 2)];
		if (c >= '0' && c <= '9' && c > best)
			best = c;
	}
	map->grid[y][x] = best;
}

int	game_init(t_game *game, const char *map_path)
{
	int	i;

	ft_memset(game, 0, sizeof(t_game));
	game->show_minimap = false;
	game->best_time = -1.0;
	if (parse_scene(map_path, game) < 0)
		return (-1);
	patch_spawn_height(&game->map);
	if (game->map.terrain && terrain_init(&game->map) < 0)
		return (-1);
	i = 0;
	while (i < game->map.height)
	{
		if (ft_strchr(game->map.grid[i], 'G'))
			game->has_goal = true;
		i++;
	}
	race_reset(game);
	i = 0;
	while (i < 4)
	{
		if (tex_load_xpm(&game->tex[i], game->map.tex_path[i]) < 0)
			return (-1);
		i++;
	}
	if (fb_init(&game->fb, WIN_WIDTH, WIN_HEIGHT) < 0)
		return (error_msg("allocation failure"));
	return (0);
}

static bool	any_move_key(t_keys *keys)
{
	return (keys->forward || keys->back || keys->strafe_l
		|| keys->strafe_r || keys->jump);
}

/*
** One deterministic physics tick. The timer is advanced in PHYS_STEP
** increments, so a run's duration is an exact tick count — replays and
** ghost verification only need the input sequence.
*/
static void	race_tick(t_game *game)
{
	char	cell;

	if (game->keys.reset)
	{
		game->keys.reset = false;
		race_reset(game);
	}
	if (game->race_state == RACE_IDLE && any_move_key(&game->keys))
		game->race_state = RACE_RUNNING;
	if (game->map.terrain)
		ski_move(game, PHYS_STEP);
	else
		move_player(game, PHYS_STEP);
	if (game->race_state != RACE_RUNNING)
		return ;
	game->run_time += PHYS_STEP;
	cell = game->map.grid[(int)game->player.y][(int)game->player.x];
	if (cell == 'G')
	{
		game->race_state = RACE_FINISHED;
		if (game->best_time < 0 || game->run_time < game->best_time)
			game->best_time = game->run_time;
	}
}

static void	update_fov(t_game *game, double dt)
{
	double	speed;
	double	target;
	double	blend;

	speed = sqrt(game->player.vx * game->player.vx
			+ game->player.vy * game->player.vy
			+ game->player.vz * game->player.vz);
	target = 1.0 + FOV_KICK * speed / SPEED_CAP;
	if (game->map.terrain)
		target = 1.0 + FOV_KICK * speed / SKI_SPEED_MAX;
	blend = 8.0 * dt;
	if (blend > 1.0)
		blend = 1.0;
	game->fov_scale += (target - game->fov_scale) * blend;
}

void	game_update(t_game *game, double dt)
{
	if (dt > 0.25)
		dt = 0.25;
	game->accum += dt;
	while (game->accum >= PHYS_STEP)
	{
		race_tick(game);
		game->accum -= PHYS_STEP;
	}
	update_fov(game, dt);
	render_frame(game);
}

void	game_destroy(t_game *game)
{
	int	i;

	i = 0;
	while (game->map.grid && i < game->map.height)
		free(game->map.grid[i++]);
	free(game->map.grid);
	i = 0;
	while (i < 4)
	{
		free(game->map.tex_path[i]);
		free(game->tex[i].pixels);
		i++;
	}
	free(game->map.hmap);
	free(game->fb.pixels);
}
