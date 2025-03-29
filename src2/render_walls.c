/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_walls.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinseo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 21:58:18 by jinseo            #+#    #+#             */
/*   Updated: 2025/03/21 21:59:50 by jinseo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	draw_walls_3d(t_data *data)
{
	int		x;
	double	ray_angle;
	double	step;
	t_ray	ray;

	step = data->fov / data->width;
	ray_angle = data->cor.dir - (data->fov / 2);
	x = 0;
	while (x < data->width)
	{
		ray = cast_single_ray(data, ray_angle);
		process_wall_slice(data, x, &ray, ray_angle);
		ray_angle += step;
		x++;
	}
}

void	process_wall_slice(t_data *data, int x, t_ray *ray, double ray_angle)
{
	double	corrected_dist;
	int		wall_height;

	corrected_dist = correct_ray_distance(ray->perp_wall_dist, ray_angle, data);
	wall_height = calculate_wall_height(data, corrected_dist);
	draw_textured_wall(data, x, *ray, wall_height);
}

double	correct_ray_distance(double corrected_dist,
		double ray_angle, t_data *data)
{
	double	correction;

	correction = cos(ray_angle - data->cor.dir);
	corrected_dist = corrected_dist * correction;
	return (corrected_dist);
}

int	calculate_wall_height(t_data *data, double corrected_dist)
{
	int	wall_height;

	wall_height = (int)(data->height / corrected_dist * 0.5);
	return (wall_height);
}

void	render_3d(t_data *data)
{
	draw_floor_ceiling(data);
	draw_walls_3d(data);
	mlx_put_image_to_window(data->mlx, data->win, data->img.img, 0, 0);
}
