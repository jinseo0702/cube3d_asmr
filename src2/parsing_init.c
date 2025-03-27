/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinseo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 20:11:38 by jinseo            #+#    #+#             */
/*   Updated: 2025/03/27 20:11:40 by jinseo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	init_t_map(t_map *map_data)
{
	map_data->fd = 0;
	map_data->high = 0;
	map_data->map_height = 1;
	map_data->map_width = 0;
	map_data->exf = 0;
	map_data->no = NULL;
	map_data->so = NULL;
	map_data->we = NULL;
	map_data->ea = NULL;
	map_data->f = 0;
	map_data->c = 0;
	map_data->map = NULL;
}

int	map_check(char *map, t_data *data)
{
	init_t_map(&data->map);
	if (ft_strrstr(map, ".cub", 4) == 0)
	{
		printf("Error Check again map Format\n");
		return (FALSE);
	}
	data->map.fd = open(map, O_RDONLY);
	if (data->map.fd < 0)
	{
		printf("Error map file not open! check map file\n");
		return (FALSE);
	}
	if (check_size(&data->map) == 0)
		return (FALSE);
	return (TRUE);
}

int	check_arg(char *str)
{
	int	idx;

	if (ft_strncmp(str, "NO ", 3) == 0)
		return (1);
	else if (ft_strncmp(str, "SO ", 3) == 0)
		return (2);
	else if (ft_strncmp(str, "WE ", 3) == 0)
		return (4);
	else if (ft_strncmp(str, "EA ", 3) == 0)
		return (8);
	else if (ft_strncmp(str, "F ", 2) == 0)
		return (16);
	else if (ft_strncmp(str, "C ", 2) == 0)
		return (32);
	else
	{
		idx = -1;
		while (str[++idx])
			if (!ft_isspace(str[idx]))
				return (0);
		return (64);
	}
}

int	check_size(t_map *map_data)
{
	int		cnt;
	char	*temp;

	cnt = 6;
	temp = get_next_line(map_data->fd);
	while (temp)
	{
		if (map_data->high == 0)
			map_data->map_width = ft_strlen(temp);
		if ((int)ft_strlen(temp) > map_data->map_width)
			map_data->map_width = ft_strlen(temp);
		map_data->exf |= check_arg(temp);
		if (check_arg(temp) && check_arg(temp) != 64)
			map_data->exf |= (1 << (++cnt));
		if (map_data->exf < 8191 && (check_arg(temp) == 0))
			printf("Error Check File Format\n");
		ft_freenull(&temp);
		map_data->high++;
		temp = get_next_line(map_data->fd);
	}
	close(map_data->fd);
	return (map_data->exf == 8191);
}

void	print_all(t_map *map)
{
	int	idx;

	printf("fd == %d, high == %d, width == %d, m_high == %d \n",
		map->fd, map->high, map->map_width, map->map_height);
	printf("NO is %s ", map->no);
	printf("SO is %s ", map->so);
	printf("WE is %s ", map->we);
	printf("EA is %s ", map->ea);
	printf("print map\n");
	idx = 0;
	while (map->map[idx])
	{
		printf("%s", map->map[idx]);
		printf("\n");
		idx++;
	}
}
