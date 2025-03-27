/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_map_data.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinseo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 21:57:46 by jinseo            #+#    #+#             */
/*   Updated: 2025/03/27 20:12:11 by jinseo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	copy_texture_path(char **dst, char *src)
{
	char	*find;
	char	*ptr;

	find = ft_strchr(src, '.');
	*dst = ft_strdup(find);
	ptr = ft_strchr(*dst, '\n');
	if (ptr)
		*ptr = '\0';
}

void	handle_texture_data(char *str, t_map *map_data, t_data *data)
{
	if (ft_strncmp(str, "NO ", 3) == 0)
		copy_texture_path(&map_data->no, str);
	else if (ft_strncmp(str, "SO ", 3) == 0)
		copy_texture_path(&map_data->so, str);
	else if (ft_strncmp(str, "WE ", 3) == 0)
		copy_texture_path(&map_data->we, str);
	else if (ft_strncmp(str, "EA ", 3) == 0)
		copy_texture_path(&map_data->ea, str);
	else if (ft_strncmp(str, "F ", 2) == 0)
		map_data->f = parse_color(str, data);
	else if (ft_strncmp(str, "C ", 2) == 0)
		map_data->c = parse_color(str, data);
}

int	dup_info(char *str, t_map *map_data, t_data *data)
{
	if (ft_onlyisspace(str) && map_data->exf > 255)
		return (FALSE);
	handle_texture_data(str, map_data, data);
	if (map_data->exf <= 127)
		validate_map_line(str, map_data);
	return (TRUE);
}

int	insert_data(t_map *map_data, char *map, t_data *data)
{
	map_data->fd = open(map, O_RDONLY);
	if (map_data->fd < 0)
		return (printf("Error Failed opne File.\n"), FALSE);
	map_data->map = (char **)ft_calloc(sizeof(char *), (map_data->high));
	map_data->map[0] = (char *)ft_calloc(sizeof(char), map_data->map_width + 2);
	ft_memset(map_data->map[0], 'X', map_data->map_width + 1);
	data->temp = get_next_line(map_data->fd);
	while (data->temp)
	{
		if (dup_info(data->temp, map_data, data) == 1 && map_data->exf ^ 127)
			map_data->exf = (map_data->exf >> 1);
		ft_freenull(&data->temp);
		data->temp = get_next_line(map_data->fd);
	}
	map_data->map[map_data->map_height] = ft_calloc(sizeof(char),
			map_data->map_width + 2);
	ft_memset(map_data->map[map_data->map_height], 'X',
		map_data->map_width + 1);
	flood_fill(map_data->map, data);
	close(map_data->fd);
	return (TRUE);
}
