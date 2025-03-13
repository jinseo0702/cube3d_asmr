/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_render.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinseo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 21:58:21 by jinseo            #+#    #+#             */
/*   Updated: 2025/03/13 21:58:22 by jinseo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void draw_textured_wall(t_data *data, int x, t_ray ray, int wall_height)
{
    t_wall_tex tex;
    int draw_start;
    int draw_end;
    t_allimg *tex_img;
    
    calculate_wall_bounds(wall_height, data, &draw_start, &draw_end);
    tex_img = select_texture(ray, data);
    
    tex.buffer = tex_img->buffer;
    tex.line_bytes = tex_img->line_bytes;
    tex.pixel_bits = tex_img->pixel_bits;
    tex.tex_x = get_tex_x(data, &ray);
    tex.height = wall_height;
    
    draw_wall_line(data, x, draw_start, draw_end, tex);
}

void setup_texture_info(t_wall_tex *tex, t_allimg *tex_img, 
                       t_data *data, t_ray *ray, int wall_height)
{
    tex->buffer = tex_img->buffer;
    tex->line_bytes = tex_img->line_bytes;
    tex->pixel_bits = tex_img->pixel_bits;
    tex->tex_x = get_tex_x(data, ray);
    tex->height = wall_height;
}

void draw_wall_line(t_data *data, int x, int start, int end, t_wall_tex tex)
{
    int y;
    int tex_y;
    double step;
    double tex_pos;
    char *dst;
    char *src;
    int color;
    
    step = (double)TEX_HEIGHT / tex.height;
    tex_pos = (start - data->height / 2 + tex.height / 2) * step;
    
    y = start;
    while (y < end)
    {
        tex_y = (int)tex_pos & (TEX_HEIGHT - 1);
        tex_pos += step;
        
        src = tex.buffer + (tex_y * tex.line_bytes + tex.tex_x * (tex.pixel_bits / 8));
        color = *(unsigned int *)src;
        
        if (y >= 0 && y < data->height && x >= 0 && x < data->width)
        {
            dst = data->img.buffer + (y * data->img.line_bytes + x * (data->img.pixel_bits / 8));
            *(unsigned int *)dst = color;
        }
        y++;
    }
}

void init_textures(t_data *data)
{
    int width;
    int height;

    data->tex_n.width = TEX_WIDTH;
    data->tex_n.height = TEX_HEIGHT;
    data->tex_s.width = TEX_WIDTH;
    data->tex_s.height = TEX_HEIGHT;
    data->tex_w.width = TEX_WIDTH;
    data->tex_w.height = TEX_HEIGHT;
    data->tex_e.width = TEX_WIDTH;
    data->tex_e.height = TEX_HEIGHT;
    
    data->north = mlx_xpm_file_to_image(data->mlx, data->map.NO, &width, &height);
    data->south = mlx_xpm_file_to_image(data->mlx, data->map.SO, &width, &height);
    data->west = mlx_xpm_file_to_image(data->mlx, data->map.WE, &width, &height);
    data->east = mlx_xpm_file_to_image(data->mlx, data->map.EA, &width, &height);
    
    data->tex_n.img = data->north;
    data->tex_s.img = data->south;
    data->tex_w.img = data->west;
    data->tex_e.img = data->east;
    
    if (data->tex_n.img)
        data->tex_n.buffer = mlx_get_data_addr(data->tex_n.img, 
        &data->tex_n.pixel_bits, &data->tex_n.line_bytes, &data->tex_n.endian);
    if (data->tex_s.img)
        data->tex_s.buffer = mlx_get_data_addr(data->tex_s.img, 
        &data->tex_s.pixel_bits, &data->tex_s.line_bytes, &data->tex_s.endian);
    if (data->tex_w.img)
        data->tex_w.buffer = mlx_get_data_addr(data->tex_w.img, 
        &data->tex_w.pixel_bits, &data->tex_w.line_bytes, &data->tex_w.endian);
    if (data->tex_e.img)
        data->tex_e.buffer = mlx_get_data_addr(data->tex_e.img, 
        &data->tex_e.pixel_bits, &data->tex_e.line_bytes, &data->tex_e.endian);
}

void load_textures(t_data *data)
{
    init_textures(data);
}
