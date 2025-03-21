/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_movement.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinseo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 21:57:53 by jinseo            #+#    #+#             */
/*   Updated: 2025/03/21 21:57:47 by jinseo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void move_player(t_data *data, int flag)
{
	double new_x;
	double new_y;

	if (flag == KEY_UP)
	{
		new_x = data->cor.x + cos(data->cor.dir) * 0.1;
		new_y = data->cor.y + sin(data->cor.dir) * 0.1;
	}
	else
	{
		new_x = data->cor.x - cos(data->cor.dir) * 0.1;
		new_y = data->cor.y - sin(data->cor.dir) * 0.1;
	}
	if (data->map.map[(int)new_y][(int)new_x] != '1' && 
			data->map.map[(int)new_y][(int)new_x] != 'X')
	{
		data->map.map[(int)data->cor.y][(int)data->cor.x] = '2';
		data->cor.x = new_x;
		data->cor.y = new_y;
		data->map.map[(int)data->cor.y][(int)data->cor.x] = data->cor.c;
	}
}



void rotate_player(t_data *data, int direction)
{
	double rotation_speed;

	rotation_speed = 0.1;
	if (direction < 0)
	{
		data->cor.dir -= rotation_speed;
		if (data->cor.dir < 0)
			data->cor.dir += 2 * M_PI;
	}
	else
	{
		data->cor.dir += rotation_speed;
		if (data->cor.dir > 2 * M_PI)
			data->cor.dir -= 2 * M_PI;
	}
}
