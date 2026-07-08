#include "../../include/cub3d.h"

static const char	*skip_spaces(const char *s)
{
	while (*s == ' ' || *s == '\t')
		s++;
	return (s);
}

static int	set_texture(t_map *map, int side, const char *value,
		int *found)
{
	if (*found & (1 << side))
		return (error_msg("duplicate texture definition"));
	map->tex_path[side] = ft_strtrim((char *)value, " \t");
	if (!map->tex_path[side] || map->tex_path[side][0] == '\0')
		return (error_msg("empty texture path"));
	*found |= (1 << side);
	return (1);
}

static int	set_color(t_map *map, int bit, const char *value, int *found)
{
	uint32_t	*target;

	if (*found & bit)
		return (error_msg("duplicate color definition"));
	if (bit == 16)
		target = &map->floor_color;
	else
		target = &map->ceil_color;
	if (parse_rgb(value, target) < 0)
		return (-1);
	*found |= bit;
	return (1);
}

/*
** Returns 1 if the line was consumed as a config element, 0 if it does not
** start with a known identifier (i.e. the map block begins), -1 on error.
*/
int	parse_config_line(char *line, t_map *map, int *found)
{
	const char	*s;

	s = skip_spaces(line);
	if (ft_strncmp(s, "NO ", 3) == 0)
		return (set_texture(map, NORTH, s + 3, found));
	if (ft_strncmp(s, "SO ", 3) == 0)
		return (set_texture(map, SOUTH, s + 3, found));
	if (ft_strncmp(s, "WE ", 3) == 0)
		return (set_texture(map, WEST, s + 3, found));
	if (ft_strncmp(s, "EA ", 3) == 0)
		return (set_texture(map, EAST, s + 3, found));
	if (ft_strncmp(s, "F ", 2) == 0)
		return (set_color(map, 16, s + 2, found));
	if (ft_strncmp(s, "C ", 2) == 0)
		return (set_color(map, 32, s + 2, found));
	return (0);
}

static int	parse_channel(const char *s)
{
	int	value;
	int	digits;

	s = skip_spaces(s);
	digits = 0;
	value = 0;
	while (ft_isdigit(s[digits]))
	{
		value = value * 10 + (s[digits] - '0');
		digits++;
	}
	if (digits == 0 || digits > 3 || value > 255)
		return (-1);
	s = skip_spaces(s + digits);
	if (*s != '\0')
		return (-1);
	return (value);
}

int	parse_rgb(const char *str, uint32_t *out)
{
	char	**parts;
	int		channel[3];
	int		i;

	parts = ft_split(str, ',');
	if (!parts)
		return (error_msg("allocation failure"));
	i = 0;
	while (i < 3 && parts[i])
	{
		channel[i] = parse_channel(parts[i]);
		i++;
	}
	if (i != 3 || parts[3] != NULL
		|| channel[0] < 0 || channel[1] < 0 || channel[2] < 0)
	{
		ft_free_two(parts);
		return (error_msg("colors must be \"R,G,B\" with values in 0-255"));
	}
	ft_free_two(parts);
	*out = 0xFF000000u | (channel[0] << 16) | (channel[1] << 8) | channel[2];
	return (0);
}
