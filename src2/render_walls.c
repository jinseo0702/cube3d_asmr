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
    
    ray_angle = data->cor.dir - (data->fov / 2);//왼쪽 30도 부터 광선을 쏜다.
    x = 0;
    while (x < data->width)
    {
        ray = cast_single_ray(data, ray_angle);
        ray.perp_wall_dist *= cos(ray_angle - data->cor.dir);
        draw_textured_wall(data, x, ray);
        ray_angle += data->fov / data->width;
        x++;
    }
}

void render_3d(t_data *data)
{
    draw_floor_ceiling(data);
    draw_walls_3d(data);//이부분
    mlx_put_image_to_window(data->mlx, data->win, data->img.img, 0, 0);
}
