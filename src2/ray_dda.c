/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_dda.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinseo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 21:57:58 by jinseo            #+#    #+#             */
/*   Updated: 2025/03/21 21:58:53 by jinseo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	perform_dda(t_ray *ray, t_data *game)
{
	char	**map;
	int		width;
	int		height;

	map = game->map.map;
	width = game->map.map_width;
	height = game->map.map_height;
	ray->hit = 0;
	while (!ray->hit)
	{
		step_to_next_grid(ray);
		if (check_wall_collision(ray, map, width, height))
			ray->hit = 1;
	}
}

int	check_wall_collision(t_ray *ray, char **map, int width, int height)
{
	if (ray->map_x < 0 || ray->map_x >= width
		|| ray->map_y < 0 || ray->map_y >= height)
	{
		ray->perp_wall_dist = 20.0;
		return (1);
	}
	if (map[ray->map_y][ray->map_x] == '1' ||
			map[ray->map_y][ray->map_x] == 'X')
		return (1);
	return (0);
}

void	step_to_next_grid(t_ray *ray)
{
	if (ray->side_dist_x < ray->side_dist_y)
	{
		ray->side_dist_x += ray->delta_dist_x;
		ray->map_x += ray->step_x;
		ray->side = 0;
	}
	else
	{
		ray->side_dist_y += ray->delta_dist_y;
		ray->map_y += ray->step_y;
		ray->side = 1;
	}
}

void	calculate_wall_distance(t_ray *ray, t_data *game)
{
	if (ray->side == 0)
		ray->perp_wall_dist = fabs((ray->map_x - game->cor.x
					+ (1 - ray->step_x) / 2) / ray->dir_x);
	else
		ray->perp_wall_dist = fabs((ray->map_y - game->cor.y
					+ (1 - ray->step_y) / 2) / ray->dir_y);
}
