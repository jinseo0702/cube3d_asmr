#include "../../include/cub3d.h"

typedef struct s_flood
{
	t_map	*map;
	bool	*visited;
	int		*stack;
	int		top;
}	t_flood;

static int	push_neighbor(t_flood *f, int x, int y)
{
	char	cell;

	if (x < 0 || x >= f->map->width || y < 0 || y >= f->map->height)
		return (error_msg("map is not closed: walkable area reaches the edge"));
	cell = f->map->grid[y][x];
	if (cell == ' ')
		return (error_msg("map is not closed: walkable area touches a void"));
	if (cell == '1' || f->visited[y * f->map->width + x])
		return (0);
	f->visited[y * f->map->width + x] = true;
	f->stack[f->top++] = y * f->map->width + x;
	return (0);
}

/*
** Iterative flood fill from the player spawn over every reachable floor
** cell. The map is closed iff the fill never steps out of bounds or onto
** a padding space.
*/
static int	flood_from_spawn(t_flood *f)
{
	int	x;
	int	y;

	x = (int)f->map->spawn_x;
	y = (int)f->map->spawn_y;
	f->visited[y * f->map->width + x] = true;
	f->stack[f->top++] = y * f->map->width + x;
	while (f->top > 0)
	{
		f->top--;
		x = f->stack[f->top] % f->map->width;
		y = f->stack[f->top] / f->map->width;
		if (push_neighbor(f, x + 1, y) < 0 || push_neighbor(f, x - 1, y) < 0
			|| push_neighbor(f, x, y + 1) < 0 || push_neighbor(f, x, y - 1) < 0)
			return (-1);
	}
	return (0);
}

int	validate_map(t_map *map)
{
	t_flood	f;
	int		ret;

	f.map = map;
	f.top = 0;
	f.visited = ft_calloc(map->width * map->height, sizeof(bool));
	f.stack = ft_calloc(map->width * map->height, sizeof(int));
	if (!f.visited || !f.stack)
	{
		free(f.visited);
		free(f.stack);
		return (error_msg("allocation failure"));
	}
	ret = flood_from_spawn(&f);
	free(f.visited);
	free(f.stack);
	return (ret);
}
