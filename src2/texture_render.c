/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_render.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinseo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 21:58:21 by jinseo            #+#    #+#             */
/*   Updated: 2025/03/22 17:39:48 by hyunahn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	draw_textured_wall(t_data *data, int x, t_ray ray, int wall_height)
{
	t_wall_tex	tex;
	t_allimg	*tex_img;

	calculate_wall_bounds(wall_height, data, &tex.draw_start, &tex.draw_end);
	tex_img = select_texture(ray, data);
	tex.buffer = tex_img->buffer;
	tex.line_bytes = tex_img->line_bytes;
	tex.pixel_bits = tex_img->pixel_bits;
	tex.tex_x = get_tex_x(data, &ray);
	tex.height = wall_height;
	draw_wall_line(data, x, tex.draw_start, tex.draw_end, tex);
}

void	draw_wall_line(t_data *data, int x, t_wall_tex tex)
{
	int		y;
	int		color;
	double	step;
	double	tex_pos;
	char	*dst;

	step = (double)TEX_HEIGHT / tex.height;
	tex_pos = (tex.draw_start - data->height / 2 + tex.height / 2) * step;
	y = tex.draw_start;
	while (y < tex.draw_end)
	{
		color = get_texture_color(&tex, tex_pos);
		tex_pos += step;
		if (y >= 0 && y < data->height && x >= 0 && x < data->width)
		{
			dst = data->img.buffer + (y * data->img.line_bytes + x \
					* (data->img.pixel_bits / 8));
			*(unsigned int *)dst = color;
		}
		++y;
	}
}

int	get_texture_color(t_wall_tex *tex, double tex_pos)
{
	int		tex_y;
	char	*src;
	int		color;

	tex_y = (int)tex_pos & (TEX_HEIGHT - 1);
	src = tex->buffer + (tex_y * tex->line_bytes + tex->tex_x \
			* (tex->pixel_bits / 8));
	color = *(unsigned int *)src;
	return (color);
}

void	init_textures(t_data *data)
{
	int	width;
	int	height;

	data->tex_n.img = mlx_xpm_file_to_image \
			(data->mlx, data->map.no, &width, &height);
	data->tex_s.img = mlx_xpm_file_to_image \
			(data->mlx, data->map.so, &width, &height);
	data->tex_w.img = mlx_xpm_file_to_image \
			(data->mlx, data->map.we, &width, &height);
	data->tex_e.img = mlx_xpm_file_to_image \
			(data->mlx, data->map.ea, &width, &height);
	if (!data->tex_n.img || !data->tex_s.img || \
			!data->tex_w.img || !data->tex_e.img)
	{
		printf("image load fail!! Check again!\n");
		exit(1);
	}
	data->tex_n.buffer = mlx_get_data_addr(data->tex_n.img, \
	&data->tex_n.pixel_bits, &data->tex_n.line_bytes, &data->tex_n.endian);
	data->tex_s.buffer = mlx_get_data_addr(data->tex_s.img, \
	&data->tex_s.pixel_bits, &data->tex_s.line_bytes, &data->tex_s.endian);
	data->tex_w.buffer = mlx_get_data_addr(data->tex_w.img, \
	&data->tex_w.pixel_bits, &data->tex_w.line_bytes, &data->tex_w.endian);
	data->tex_e.buffer = mlx_get_data_addr(data->tex_e.img, \
	&data->tex_e.pixel_bits, &data->tex_e.line_bytes, &data->tex_e.endian);
}
