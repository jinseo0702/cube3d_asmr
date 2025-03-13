/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_walls.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinseo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 21:58:18 by jinseo            #+#    #+#             */
/*   Updated: 2025/03/13 21:58:20 by jinseo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void draw_walls_3d(t_data *data)
{
    double ray_angle;
    int x;
    t_ray ray;
    int wall_height;
    double corrected_dist;
    
    ray_angle = data->cor.dir - (data->fov / 2);
    x = 0;
    
    while (x < data->width)
    {
        ray = cast_single_ray(data, ray_angle);
        
        corrected_dist = ray.perp_wall_dist;
        correct_ray_distance(&corrected_dist, ray_angle, data);
        
        wall_height = calculate_wall_height(data, corrected_dist);
        
        draw_textured_wall(data, x, ray, wall_height);
        
        ray_angle += data->fov / data->width;
        x++;
    }
}

void correct_ray_distance(double *corrected_dist, double ray_angle, t_data *data)
{
    double correction;
    
    correction = cos(ray_angle - data->cor.dir);
    *corrected_dist = *corrected_dist * correction;
    
    if (*corrected_dist < 0.1)
        *corrected_dist = 0.1;
}

int calculate_wall_height(t_data *data, double corrected_dist)
{
    int wall_height;
    
    wall_height = (int)(data->height / corrected_dist * 0.5);
    return (wall_height);
}

void render_3d(t_data *data)
{
    draw_floor_ceiling(data);
    draw_walls_3d(data);
    mlx_put_image_to_window(data->mlx, data->win, data->img.img, 0, 0);
}

double get_wall_x(t_data *data, t_ray *ray)
{
    double wall_x;
    
    if (ray->side == 0)
        wall_x = data->cor.y + ray->perp_wall_dist * ray->dir_y;
    else
        wall_x = data->cor.x + ray->perp_wall_dist * ray->dir_x;
    
    wall_x -= floor(wall_x);
    
    return (wall_x);
}
