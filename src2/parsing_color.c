/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_color.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinseo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 21:57:33 by jinseo            #+#    #+#             */
/*   Updated: 2025/03/13 21:57:34 by jinseo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

// RGB 값 유효성 검사 함수
int validate_rgb(int r, int g, int b)
{
    if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
        return (0);
    return (1);
}

// RGB 문자열 분리 함수
char **split_rgb_parts(char *color_str)
{
    char **rgb;
    
    rgb = ft_split(color_str, ',');
    if (!rgb || ft_arraylen(rgb) != 3)
    {
        if (rgb)
            ft_free_array(rgb);
        return (NULL);
    }
    return (rgb);
}

// 색상 파싱을 위한 문자열 분리 함수
char **split_color_line(char *line)
{
    char **parts;
    
    parts = ft_split(line, ' ');
    if (!parts || ft_arraylen(parts) < 2)
    {
        if (parts)
            ft_free_array(parts);
        return (NULL);
    }
    return (parts);
}

// RGB 값으로부터 색상 생성 함수
int create_color(int r, int g, int b)
{
    return ((r << 16) | (g << 8) | b);
}

// 색상 문자열 파싱 메인 함수
int parse_color(char *line)
{
    int r;
    int g;
    int b;
    char **parts;
    char **rgb;

    parts = split_color_line(line);
    if (!parts)
        return (0x000000);

    rgb = split_rgb_parts(parts[1]);
    ft_free_array(parts);

    if (!rgb)
        return (0x000000);

    r = ft_atoi(rgb[0]);
    g = ft_atoi(rgb[1]);
    b = ft_atoi(rgb[2]);

    ft_free_array(rgb);

    if (!validate_rgb(r, g, b))
        return (0x000000);

    return (create_color(r, g, b));
}
