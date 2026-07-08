#include "../../include/cub3d.h"

int	error_msg(const char *msg)
{
	ft_putstr_fd("Error\n", 2);
	ft_putstr_fd((char *)msg, 2);
	ft_putstr_fd("\n", 2);
	return (-1);
}

static bool	is_blank(const char *line)
{
	while (*line)
	{
		if (*line != ' ' && *line != '\t')
			return (false);
		line++;
	}
	return (true);
}

static int	check_extension(const char *path)
{
	size_t	len;

	len = ft_strlen(path);
	if (len < 5 || ft_strncmp(path + len - 4, ".cub", 4) != 0)
		return (error_msg("map file must have a .cub extension"));
	return (0);
}

/*
** Reads the configuration block (NO/SO/WE/EA texture paths and F/C colors),
** then treats everything below it as the map block. Blank lines are allowed
** around the config but not inside the map.
*/
static int	split_blocks(char **lines, int count, t_map *map, int *map_start)
{
	int	i;
	int	found;
	int	ret;

	i = 0;
	found = 0;
	while (i < count && found != 63)
	{
		if (is_blank(lines[i]))
		{
			i++;
			continue ;
		}
		ret = parse_config_line(lines[i], map, &found);
		if (ret < 0)
			return (-1);
		if (ret == 0)
			return (error_msg("map must come after all six config elements"));
		i++;
	}
	if (found != 63)
		return (error_msg("missing config element (NO, SO, WE, EA, F, C)"));
	while (i < count && is_blank(lines[i]))
		i++;
	if (i == count)
		return (error_msg("map is missing"));
	*map_start = i;
	return (0);
}

int	parse_scene(const char *path, t_game *game)
{
	char	**lines;
	int		count;
	int		start;
	int		end;

	if (check_extension(path) < 0)
		return (-1);
	lines = read_all_lines(path, &count);
	if (!lines || count == 0)
		return (free_lines(lines), error_msg("cannot read map file"));
	if (split_blocks(lines, count, &game->map, &start) < 0)
		return (free_lines(lines), -1);
	end = count;
	while (end > start && is_blank(lines[end - 1]))
		end--;
	if (build_grid(lines, start, end - start, &game->map) < 0)
		return (free_lines(lines), -1);
	free_lines(lines);
	if (validate_map(&game->map) < 0)
		return (-1);
	return (0);
}
