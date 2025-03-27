/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_map_check.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinseo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 21:57:43 by jinseo            #+#    #+#             */
/*   Updated: 2025/03/27 20:11:57 by jinseo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

int	is_valid_map_char(char c)
{
	return (ft_isinstr(c, "01NSWE ") != 0);
}

void	copy_and_process_map_line(t_map *map_data, char *str)
{
	int	idx;

	map_data->map[map_data->map_height] = ft_calloc(1, map_data->map_width + 2);
	ft_memset(map_data->map[map_data->map_height],
		'X', map_data->map_width + 1);
	ft_strlcpy(&map_data->map[map_data->map_height][1],
		str, ft_strlen(str) + 2);
	idx = -1;
	while (map_data->map[map_data->map_height][++idx])
		if (ft_isspace(map_data->map[map_data->map_height][idx]))
			map_data->map[map_data->map_height][idx] = 'X';
	if ((int)ft_strlen(str) < map_data->map_width)
		map_data->map[map_data->map_height][ft_strlen(str) + 1] = 'X';
	else
		map_data->map[map_data->map_height][map_data->map_width] = 'X';
	map_data->map_height++;
}

void	validate_map_line(char *str, t_map *map_data)
{
	int	idx;

	idx = -1;
	while (str[++idx] && str[idx] != '\n')
	{
		if (!is_valid_map_char(str[idx]))
		{
			printf("Error : %c is not correct format in map\n", str[idx]);
			exit(1);
		}
	}
	copy_and_process_map_line(map_data, str);
}
