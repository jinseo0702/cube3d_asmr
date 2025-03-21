/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinseo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 21:57:14 by jinseo            #+#    #+#             */
/*   Updated: 2025/03/21 21:56:14 by jinseo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void setup_events(t_data *data)
{
	mlx_hook(data->win, 2, 1L<<0, ft_key_handling, data);
	mlx_hook(data->win, 17, 0, ft_exit_handling, data);
}

int ft_exit_handling(void *param)
{
	t_data *data;

	data = (t_data *)param;
	mlx_destroy_image(data->mlx, data->img.img);
	mlx_destroy_window(data->mlx, data->win);
	mlx_destroy_display(data->mlx);
	free(data->mlx);
	exit(0);
}

void update_view(t_data *data)
{
	render_3d(data);
}

int handle_movement_keys(int keycode, t_data *data)
{
	if (keycode == KEY_UP || keycode == KEY_W)
		move_player(data, KEY_UP);
	else if (keycode == KEY_DOWN || keycode == KEY_S)
		move_player(data, KEY_DOWN);
	else if (keycode == KEY_LEFT || keycode == KEY_A)
		rotate_player(data, -1);
	else if (keycode == KEY_RIGHT || keycode == KEY_D)
		rotate_player(data, 1);
	else
		return (0);
	return (1);
}

int ft_key_handling(int keycode, t_data *data)
{
	if (keycode == KEY_ESCAPE)
		ft_exit_handling(data);
	else if (handle_movement_keys(keycode, data))
		render_3d(data);
	return (0);
}
