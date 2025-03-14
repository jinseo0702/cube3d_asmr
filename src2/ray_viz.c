/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_viz.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinseo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 21:58:09 by jinseo            #+#    #+#             */
/*   Updated: 2025/03/13 21:58:10 by jinseo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void draw_rays(t_data *data)
{
    double ray_angle;
    int i;
    double player_dir;
    
    player_dir = -M_PI / 2;
    ray_angle = player_dir - (data->fov / 2);
    
    i = 0;
    while (i < data->ray_count)
    {
        draw_single_ray(data, ray_angle);
        ray_angle += data->fov / data->ray_count;
        i++;
    }
}

double limit_ray_length(double ray_length, t_data *data)
{
    if (ray_length > data->map.map_width || ray_length > data->map.map_height)
    {
        if (data->map.map_width < data->map.map_height)
            return (data->map.map_width);
        else
            return (data->map.map_height);
    }
    return (ray_length);
}

void calculate_ray_endpoint(int start_x, int start_y, int *end_x, int *end_y, 
                         double ray_angle, double ray_length)
{
    *end_x = start_x + cos(ray_angle) * ray_length * TILE_SIZE;
    *end_y = start_y + sin(ray_angle) * ray_length * TILE_SIZE;
}

void draw_single_ray(t_data *data, double ray_angle)
{
    t_ray ray;
    int start_x;
    int start_y;
    int end_x;
    int end_y;
    double ray_length;
    
    start_x = (data->cor.x * TILE_SIZE);
    start_y = (data->cor.y * TILE_SIZE);
    
    ray = cast_single_ray(data, ray_angle);
    ray_length = ray.perp_wall_dist;
    ray_length = limit_ray_length(ray_length, data);
    
    calculate_ray_endpoint(start_x, start_y, &end_x, &end_y, 
                        ray_angle, ray_length);
    
    draw_line_to_image(data, start_x, start_y, end_x, end_y, 0xFFFF33);
}
