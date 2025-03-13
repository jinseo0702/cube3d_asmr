/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_draw.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinseo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 21:57:28 by jinseo            #+#    #+#             */
/*   Updated: 2025/03/13 21:57:30 by jinseo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void draw_line_to_image(t_data *data, int x1, int y1, int x2, int y2, int color)
{
    process_line_algorithm(data, x1, y1, x2, y2, color);
}

void init_line_draw(int *dx, int *dy, int *sx, int *sy, 
                  int x1, int y1, int x2, int y2)
{
    *dx = abs(x2 - x1);
    *dy = -abs(y2 - y1);
    
    if (x1 < x2)
        *sx = 1;
    else
        *sx = -1;
        
    if (y1 < y2)
        *sy = 1;
    else
        *sy = -1;
}

void set_line_pixel(t_data *data, int x, int y, int color)
{
    char *dst;
    
    if (x >= 0 && x < data->width && y >= 0 && y < data->height)
    {
        dst = data->img.buffer + (y * data->img.line_bytes) + 
            (x * (data->img.pixel_bits / 8));
        *(unsigned int *)dst = color;
    }
}

void process_line_algorithm(t_data *data, int x1, int y1, 
                         int x2, int y2, int color)
{
    int dx;
    int dy;
    int sx;
    int sy;
    int err;
    int e2;
    
    init_line_draw(&dx, &dy, &sx, &sy, x1, y1, x2, y2);
    err = dx + dy;
    
    while (1)
    {
        set_line_pixel(data, x1, y1, color);
        
        if (x1 == x2 && y1 == y2)
            break;
            
        e2 = 2 * err;
        if (e2 >= dy)
        {
            if (x1 == x2)
                break;
            err += dy;
            x1 += sx;
        }
        if (e2 <= dx)
        {
            if (y1 == y2)
                break;
            err += dx;
            y1 += sy;
        }
    }
}

void draw_square_to_image(t_data *data, int x, int y, int color)
{
    int i;
    int j;
    int pixel_x;
    int pixel_y;
    char *dst;

    pixel_x = x * TILE_SIZE;
    pixel_y = y * TILE_SIZE;
    
    i = 0;
    while (i < TILE_SIZE)
    {
        j = 0;
        while (j < TILE_SIZE)
        {
            dst = data->img.buffer + ((pixel_y + i) * data->img.line_bytes) + 
                ((pixel_x + j) * (data->img.pixel_bits / 8));
            *(unsigned int *)dst = color;
            j++;
        }
        i++;
    }
}
