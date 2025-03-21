/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_dda.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinseo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 21:57:58 by jinseo            #+#    #+#             */
/*   Updated: 2025/03/13 21:58:08 by jinseo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void calculate_step_side_dist_y(t_ray *ray, t_data *game)
{
    if (ray->dir_y < 0)
    {
        ray->step_y = -1;
        ray->side_dist_y = (game->cor.y - ray->map_y) * ray->delta_dist_y;
    }
    else
    {
        ray->step_y = 1;
        ray->side_dist_y = (ray->map_y + 1.0 - game->cor.y) * ray->delta_dist_y;
    }
}

int check_wall_hit(t_ray *ray, t_data *game)
{
    if (game->map.map[ray->map_y][ray->map_x] == '1' || 
        game->map.map[ray->map_y][ray->map_x] == 'X')
    {
        ray->hit = 1;
        return (TRUE);
    }
    return (FALSE);
}

void perform_dda(t_ray *ray, t_data *game)
{
    ray->hit = 0;
    while (ray->hit == 0)
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
        check_wall_hit(ray, game);
    }
    if (ray->side == 0)
        ray->perp_wall_dist = (ray->map_x - game->cor.x + (1 - ray->step_x) / 2) / ray->dir_x;
    else
        ray->perp_wall_dist = (ray->map_y - game->cor.y + (1 - ray->step_y) / 2) / ray->dir_y;
}
