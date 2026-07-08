#include "../../include/cub3d.h"

static int	register_player(t_map *map, int x, int y)
{
	if (map->spawn_dir != '\0')
		return (error_msg("map must contain exactly one player (N, S, E, W)"));
	map->spawn_dir = map->grid[y][x];
	map->spawn_x = x + 0.5;
	map->spawn_y = y + 0.5;
	map->grid[y][x] = '0';
	return (0);
}

static int	scan_cells(t_map *map)
{
	int	x;
	int	y;

	y = 0;
	while (y < map->height)
	{
		x = 0;
		while (x < map->width)
		{
			if (!ft_isinstr(map->grid[y][x], "01NSEW "))
				return (error_msg("map may only contain 0, 1, N, S, E, W"));
			if (ft_isinstr(map->grid[y][x], "NSEW")
				&& register_player(map, x, y) < 0)
				return (-1);
			x++;
		}
		y++;
	}
	if (map->spawn_dir == '\0')
		return (error_msg("map has no player spawn (N, S, E or W)"));
	return (0);
}

/*
** Copies the map block into a rectangular grid padded with spaces, then
** checks the character set and extracts the single player spawn.
*/
int	build_grid(char **lines, int start, int count, t_map *map)
{
	int	y;
	int	x;

	map->height = count;
	map->width = 0;
	y = 0;
	while (y < count)
	{
		if (lines[start + y][0] == '\0')
			return (error_msg("map must not contain blank lines"));
		if ((int)ft_strlen(lines[start + y]) > map->width)
			map->width = ft_strlen(lines[start + y]);
		y++;
	}
	map->grid = ft_calloc(map->height + 1, sizeof(char *));
	if (!map->grid)
		return (error_msg("allocation failure"));
	y = -1;
	while (++y < count)
	{
		map->grid[y] = malloc(map->width + 1);
		if (!map->grid[y])
			return (error_msg("allocation failure"));
		x = -1;
		while (++x < map->width)
			map->grid[y][x] = ' ';
		map->grid[y][map->width] = '\0';
		ft_memcpy(map->grid[y], lines[start + y], ft_strlen(lines[start + y]));
	}
	return (scan_cells(map));
}
