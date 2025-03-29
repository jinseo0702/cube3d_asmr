/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinseo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 21:57:14 by jinseo            #+#    #+#             */
/*   Updated: 2025/03/22 17:45:04 by hyunahn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	setup_events(t_data *data)
{
	mlx_hook(data->win, 2, 1L << 0, ft_key_handling, data);
	mlx_hook(data->win, 17, 0, ft_exit_handling, data);
}

int	ft_exit_handling(void *param)
{
	t_data	*data;

	data = (t_data *)param;
	free_data(data);
	if (data->tex_n.img)
		mlx_destroy_image(data->mlx, data->tex_n.img);
	if (data->tex_s.img)
		mlx_destroy_image(data->mlx, data->tex_s.img);
	if (data->tex_w.img)
		mlx_destroy_image(data->mlx, data->tex_w.img);
	if (data->tex_e.img)
		mlx_destroy_image(data->mlx, data->tex_e.img);
	if (data->img.img && data->mlx)
		mlx_destroy_image(data->mlx, data->img.img);
	if (data->win && data->mlx)
		mlx_destroy_window(data->mlx, data->win);
	if (data->mlx)
	{
		mlx_destroy_display(data->mlx);
		free(data->mlx);
	}
	exit(0);
}

int	handle_movement_keys(int keycode, t_data *data)
{
	if (keycode == KEY_UP || keycode == KEY_W)
		move_player(data, KEY_UP);
	else if (keycode == KEY_DOWN || keycode == KEY_S)
		move_player(data, KEY_DOWN);
	else if (keycode == KEY_A)
		move_player2(data, KEY_A);
	else if (keycode == KEY_D)
		move_player2(data, KEY_D);
	else if (keycode == KEY_LEFT)
		rotate_player(data, -1);
	else if (keycode == KEY_RIGHT)
		rotate_player(data, 1);
	else
		return (0);
	return (1);
}

int	ft_key_handling(int keycode, t_data *data)
{
	if (keycode == KEY_ESCAPE)
		ft_exit_handling(data);
	else if (handle_movement_keys(keycode, data))
		render_3d(data);
	return (0);
}

void	free_data(t_data *data)
{
	ft_freenull(&data->map.no);
	ft_freenull(&data->map.so);
	ft_freenull(&data->map.we);
	ft_freenull(&data->map.ea);
	ft_freenull(&data->temp);
	ft_free_two(data->map.map);
}
