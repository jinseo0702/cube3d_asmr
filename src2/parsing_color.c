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
void validate_rgb(int r, int g, int b)
{
    if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
    {
        printf("Check range is not cool check again\n");
        exit(1);
    }
}

// RGB 문자열 분리 함수
char **split_rgb_parts(char *color_str)
{
    char **rgb;
    
    rgb = ft_split(color_str, ',');
    if (ft_arraylen(rgb) != 3)
    {
        ft_free_array(rgb);
        printf("Check color Syntex!\n");
        exit(1);
    }
    return (rgb);
}

// 색상 파싱을 위한 문자열 분리 함수
char **split_color_line(char *line)
{
    char **parts;
    
    parts = ft_split(line, ' ');    
    if (ft_arraylen(parts) != 2)
    {
        ft_free_array(parts);
        printf("Check color Syntex!\n");
        exit(1);
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
    rgb = split_rgb_parts(parts[1]);
    ft_free_array(parts);
    r = ft_atoi(rgb[0]);
    g = ft_atoi(rgb[1]);
    b = ft_atoi(rgb[2]);
    ft_free_array(rgb);
    validate_rgb(r, g, b);
    return (create_color(r, g, b));
}
