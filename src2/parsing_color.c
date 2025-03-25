/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_color.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinseo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 21:57:33 by jinseo            #+#    #+#             */
/*   Updated: 2025/03/21 21:56:54 by jinseo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	validate_rgb(int r, int g, int b, t_data *data)
{
	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
	{
		printf("Check range is not cool check again\n");
		ft_exit_handling(data);
	}
}

char	**split_rgb_parts(char *color_str, t_data *data, char **str)
{
	char	**rgb;

	rgb = ft_split(color_str, ',');
	if (ft_arraylen(rgb) != 3)
	{
		ft_free_array(rgb);
		ft_free_array(str);
		printf("Check color Syntex!\n");
		ft_exit_handling(data);
	}
	return (rgb);
}

char	**split_color_line(char *line, t_data *data)
{
	char	**parts;

	parts = ft_split(line, ' ');
	if (ft_arraylen(parts) != 2)
	{
		ft_free_array(parts);
		printf("Check color Syntex!\n");
		ft_exit_handling(data);
	}
	return (parts);
}

int	create_color(int r, int g, int b)
{
	return ((r << 16) | (g << 8) | b);
}

int	parse_color(char *line, t_data *data)
{
	int		r;
	int		g;
	int		b;
	char	**parts;
	char	**rgb;

	parts = split_color_line(line, data);
	rgb = split_rgb_parts(parts[1], data, parts);
	ft_free_array(parts);
	r = ft_atoi(rgb[0]);
	g = ft_atoi(rgb[1]);
	b = ft_atoi(rgb[2]);
	ft_free_array(rgb);
	validate_rgb(r, g, b, data);
	return (create_color(r, g, b));
}
