/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_casting.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinseo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 21:57:56 by jinseo            #+#    #+#             */
/*   Updated: 2025/03/13 21:57:58 by jinseo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

t_ray cast_single_ray(t_data *game, double angle)
{
    t_ray ray;
    
    init_ray_direction(&ray, angle);
    init_ray_map_pos(&ray, game);
    calculate_delta_dist(&ray);
    calculate_step_side_dist_x(&ray, game);
    calculate_step_side_dist_y(&ray, game);
    perform_dda(&ray, game);
    calculate_wall_distance(&ray, game);
    
    return (ray);
}

void init_ray_direction(t_ray *ray, double angle)
{
    ray->dir_x = cos(angle);
    ray->dir_y = sin(angle);
}

void init_ray_map_pos(t_ray *ray, t_data *game)
{
    ray->map_x = (int)game->cor.x;
    ray->map_y = (int)game->cor.y;
}

void calculate_delta_dist(t_ray *ray)
{
    if (ray->dir_x == 0)
        ray->delta_dist_x = 1e30;
    else
        ray->delta_dist_x = fabs(1 / ray->dir_x);
    if (ray->dir_y == 0)
        ray->delta_dist_y = 1e30;
    else
        ray->delta_dist_y = fabs(1 / ray->dir_y);
}

void calculate_step_side_dist_x(t_ray *ray, t_data *game)
{
    if (ray->dir_x < 0)
    {
        ray->step_x = -1;
        ray->side_dist_x = (game->cor.x - ray->map_x) * ray->delta_dist_x;
    }
    else
    {
        ray->step_x = 1;
        ray->side_dist_x = (ray->map_x + 1.0 - game->cor.x) * ray->delta_dist_x;
    }
}
