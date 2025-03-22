/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinseo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 21:58:23 by jinseo            #+#    #+#             */
/*   Updated: 2025/03/21 22:00:31 by jinseo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

int	get_tex_x(t_data *data, t_ray *ray)
{
	double	wall_x;
	int		tex_x;

	if (ray->side == 0)
		wall_x = data->cor.y + ray->perp_wall_dist * ray->dir_y;
	else
		wall_x = data->cor.x + ray->perp_wall_dist * ray->dir_x;
	wall_x -= floor(wall_x);
	tex_x = (int)(wall_x * (double)TEX_WIDTH);
	if ((ray->side == 0 && ray->dir_x > 0)
		|| (ray->side == 1 && ray->dir_y < 0))
		tex_x = TEX_WIDTH - tex_x - 1;
	return (tex_x);
}

void	calculate_wall_bounds(int wall_height, t_data *data,
		int *draw_start, int *draw_end)
{
	*draw_start = -wall_height / 2 + data->height / 2;
	if (*draw_start < 0)
		*draw_start = 0;
	*draw_end = wall_height / 2 + data->height / 2;
	if (*draw_end >= data->height)
		*draw_end = data->height - 1;
}

t_allimg	*select_texture(t_ray ray, t_data *data)
{
	if (ray.side == 0)
	{
		if (ray.dir_x > 0)
			return (&data->tex_e);
		else
			return (&data->tex_w);
	}
	else
	{
		if (ray.dir_y > 0)
			return (&data->tex_s);
		else
			return (&data->tex_n);
	}
}
