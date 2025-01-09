/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunahn <hyunahn@student.42gyeongsan.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 22:49:03 by hyunahn           #+#    #+#             */
/*   Updated: 2025/01/07 22:53:01 by hyunahn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void put_pixel_to_image(t_allimg *img, int x, int y, int color)
{
    char *dst;

    if (x < 0 || x >= img->width || y < 0 || y >= img->height)
        return;
    dst = img->buffer + (y * img->line_bytes + x * (img->pixel_bits / 8));
    dst[0] = color & 0xFF;
    dst[1] = (color >> 8) & 0xFF;
    dst[2] = (color >> 16) & 0xFF;
    dst[3] = 0xFF;
}

void init_draw_info(t_draw_func *draw, t_two_coordi_node p1, t_two_coordi_node p2)
{
    draw->dx = abs(p2.x - p1.x);
    draw->dy = abs(p2.y - p1.y);
    draw->err = draw->dx - draw->dy;

    if (p1.x < p2.x)
        draw->sx = 1;
    else
        draw->sx = -1;

    if (p1.y < p2.y)
        draw->sy = 1;
    else
        draw->sy = -1;
}


void draw_line(t_two_coordi_node p1, t_two_coordi_node p2, t_data *data, int color)
{
    t_draw_func draw;

    init_draw_info(&draw, p1, p2);
    while (1)
    {
        put_pixel_to_image(&data->img, p1.x + data->x_offset, p1.y + data->y_offset, color);
        if (p1.x == p2.x && p1.y == p2.y)
            break;
        draw.e2 = draw.err * 2;
        if (draw.e2 > -draw.dy)
	{
            draw.err -= draw.dy;
            p1.x += draw.sx;
        }
        if (draw.e2 < draw.dx)
	{
            draw.err += draw.dx;
            p1.y += draw.sy;
        }
    }
}

void draw_map_from_array(t_data *data)
{
    int	y;
    int x;

    y = 0;
    // print_all(&data->map);
    while (y < data->map.map_height)
    {
        x = 0;
        while (x < data->map.map_width)
	{
            if (x + 1 < data->map.map_width)
	    {
                t_two_coordi_node p1 = {x, y, 0xffa500};
                t_two_coordi_node p2 = {x + 1, y, 0xffa500};
                draw_line(p1, p2, data, 0xffa500);
            }
            if (y + 1 < data->map.map_height)
	    {
                t_two_coordi_node p1 = {x, y, 0xffa500};
                t_two_coordi_node p2 = {x, y + 1, 0xffa500};
                draw_line(p1, p2, data, 0xffa500);
            }
            x++;
        }
        y++;
    }
}

