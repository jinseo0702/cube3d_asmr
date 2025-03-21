/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_map_data.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinseo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 21:57:46 by jinseo            #+#    #+#             */
/*   Updated: 2025/03/21 21:57:21 by jinseo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void copy_texture_path(char **dst, char *src)
{
	char *find;

	find = ft_strchr(src, '.');
	*dst = ft_strdup(find);
	char *ptr = ft_strchr(*dst, '\n');
	if (ptr)
		*ptr = '\0';
}


void handle_texture_data(char *str, t_map *map_data)
{
	if (ft_strncmp(str, "NO ", 3) == 0)
		copy_texture_path(&map_data->NO, str);
	else if (ft_strncmp(str, "SO ", 3) == 0)
		copy_texture_path(&map_data->SO, str);
	else if (ft_strncmp(str, "WE ", 3) == 0)
		copy_texture_path(&map_data->WE, str);
	else if (ft_strncmp(str, "EA ", 3) == 0)
		copy_texture_path(&map_data->EA, str);
	else if (ft_strncmp(str, "F ", 2) == 0)
		map_data->F = parse_color(str);
	else if (ft_strncmp(str, "C ", 2) == 0)
		map_data->C = parse_color(str);
}


int dup_info(char *str, t_map *map_data)
{
	if (ft_onlyisspace(str) && map_data->exf > 255)
		return (FALSE);
	printf("%s, %d", str, map_data->exf);
	handle_texture_data(str, map_data);
	if(map_data->exf <= 127)
		is_right_map(str, map_data);
	return (TRUE);
}


int insert_data(t_map *map_data, char *map)
{
	char *temp;

	map_data->fd = open(map, O_RDONLY);
	if (map_data->fd < 0)
	{
		printf("Error Failed opne File.\n");
		return (FALSE);
	}
	map_data->map = (char **)ft_calloc(sizeof(char *), (map_data->high));
	map_data->map[0] = (char *)ft_calloc(sizeof(char), map_data->map_width + 2);
	ft_memset(map_data->map[0], 'X', map_data->map_width + 1);
	while ((temp = get_next_line(map_data->fd)))
	{
		if(dup_info(temp, map_data) == 1 && map_data->exf^127)
			map_data->exf = (map_data->exf >> 1);
		ft_freenull(&temp);
	}
	map_data->map[map_data->map_height] = ft_calloc(sizeof(char), 
			map_data->map_width + 2);
	ft_memset(map_data->map[map_data->map_height], 'X', map_data->map_width + 1);
	flood_fill(map_data->map);
	print_all(map_data);
	close(map_data->fd);
	return (1);
}
