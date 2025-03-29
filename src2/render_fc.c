/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_fc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinseo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 21:58:11 by jinseo            #+#    #+#             */
/*   Updated: 2025/03/21 22:03:41 by jinseo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	draw_floor_ceiling(t_data *data)
{
	int		x;
	int		y;
	char	*dst;
	int		color;

	y = 0;
	while (y < data->height)
	{
		if (y < (data->height / 2))
			color = data->map.c;
		else
			color = data->map.f;
		x = 0;
		while (x < data->width)
		{
			dst = data->img.buffer + (y * data->img.line_bytes)
				+ (x * (data->img.pixel_bits / 8));
			*(unsigned int *)dst = color;
			x++;
		}
		y++;
	}
}

void	init_player_direction(t_data *data)
{
	if (data->cor.c == 'N')
		data->cor.dir = -M_PI / 2;
	else if (data->cor.c == 'S')
		data->cor.dir = M_PI / 2;
	else if (data->cor.c == 'E')
		data->cor.dir = 0;
	else if (data->cor.c == 'W')
		data->cor.dir = M_PI;
}

void	find_obj(t_data *data)
{
	int			x;
	int			y;
	static int	cnt;

	y = -1;
	while (++y < data->map.map_height)
	{
		x = -1;
		while (++x < data->map.map_width)
		{
			if ((ft_isinstr(data->map.map[y][x], "NSWE")))
			{
				data->cor.c = data->map.map[y][x];
				data->cor.x = x + 0.5;
				data->cor.y = y + 0.5;
				cnt++;
			}
		}
	}
	if (cnt > 1 || cnt == 0)
	{
		printf("Chrater is only one! Check again!\n");
		exit(1);
	}
	solve_dfs2(data->map.map, data->cor.y, data->cor.x);
}

void	solve_dfs2(char **map, int x, int y)
{
	if (map[x][y] == 'X')
	{
		printf("Error  Map style is not Good!");
		exit (1);
	}
	else if (map[x][y] == '2' || map[x][y] == '1')
		return ;
	solve_dfs2(map, (x - 1), y);
	solve_dfs2(map, (x + 1), y);
	solve_dfs2(map, x, (y - 1));
	solve_dfs2(map, x, (y + 1));
}
