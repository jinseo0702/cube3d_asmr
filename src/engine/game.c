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

int	game_init(t_game *game, const char *map_path)
{
	int	i;

	ft_memset(game, 0, sizeof(t_game));
	game->show_minimap = true;
	if (parse_scene(map_path, game) < 0)
		return (-1);
	game->player.x = game->map.spawn_x;
	game->player.y = game->map.spawn_y;
	set_direction(&game->player, game->map.spawn_dir);
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

void	game_update(t_game *game, double dt)
{
	move_player(game, dt);
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
	free(game->fb.pixels);
}
