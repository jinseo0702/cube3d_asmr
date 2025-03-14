/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_fc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinseo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 21:58:11 by jinseo            #+#    #+#             */
/*   Updated: 2025/03/13 21:58:15 by jinseo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void draw_floor_ceiling(t_data *data)
{
    draw_ceiling(data);
    draw_floor(data);
}

void draw_ceiling(t_data *data)
{
    int x;
    int y;
    char *dst;

    y = 0;
    while (y < data->height / 2)
    {
        x = 0;
        while (x < data->width)
        {
            dst = data->img.buffer + (y * data->img.line_bytes) 
                + (x * (data->img.pixel_bits / 8));
            *(unsigned int *)dst = data->map.C;
            x++;
        }
        y++;
    }
}

void draw_floor(t_data *data)
{
    int x;
    int y;
    char *dst;

    y = data->height / 2;
    while (y < data->height)
    {
        x = 0;
        while (x < data->width)
        {
            dst = data->img.buffer + (y * data->img.line_bytes) 
                + (x * (data->img.pixel_bits / 8));
            *(unsigned int *)dst = data->map.F;
            x++;
        }
        y++;
    }
}

void init_player_direction(t_data *data)
{
    if (data->cor.c == 'N')
        data->cor.dir = -M_PI / 2;
    else if (data->cor.c == 'S')
        data->cor.dir = M_PI / 2;
    else if (data->cor.c == 'E')
        data->cor.dir = 0;
    else if (data->cor.c == 'W')
        data->cor.dir = M_PI;
}

void find_obj(t_data *data)
{
    int x;
    int y;
    static int cnt;
    
    data->cor.color = 0xFF0000;
    y = -1;
    while (++y < data->map.map_height)
    {
        x = -1;
        while (++x < data->map.map_width)
        {
            if ((ft_isinstr(data->map.map[y][x], "NSWE")))
            {
                data->cor.c = data->map.map[y][x];
                data->cor.x = x;
                data->cor.y = y;
                cnt++;
                if (cnt > 1)
                    data->status -= 1;
            }
        }
    }
}
