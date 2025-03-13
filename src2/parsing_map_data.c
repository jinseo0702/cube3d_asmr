/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_map_data.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinseo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 21:57:46 by jinseo            #+#    #+#             */
/*   Updated: 2025/03/13 21:57:53 by jinseo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

// 텍스처 경로 값 복사 함수
void copy_texture_path(char **dst, char *src, int *status)
{
    *dst = ft_strdup_flag(src, status);
    char *ptr = ft_strchr(*dst, '\n');
    if (ptr)
        *ptr = '\0';
}

// 텍스처 파일 관련 데이터 저장 함수
int handle_texture_data(char *str, t_map *map_data)
{
    int status;
    
    status = 0;
    if (ft_strncmp(str, "NO ", 3) == 0)
    {
        copy_texture_path(&map_data->NO, str + 3, &status);
    }
    else if (ft_strncmp(str, "SO ", 3) == 0)
    {
        copy_texture_path(&map_data->SO, str + 3, &status);
    }
    else if (ft_strncmp(str, "WE ", 3) == 0)
    {
        copy_texture_path(&map_data->WE, str + 3, &status);
    }
    else if (ft_strncmp(str, "EA ", 3) == 0)
    {
        copy_texture_path(&map_data->EA, str + 3, &status);
    }
    return (status);
}

// 색상 데이터 저장 함수
int handle_color_data(char *str, t_map *map_data)
{
    if (ft_strncmp(str, "F ", 2) == 0)
    {
        map_data->F = parse_color(str);
        return (0);
    }
    else if (ft_strncmp(str, "C ", 2) == 0)
    {
        map_data->C = parse_color(str);
        return (0);
    }
    return (-1);
}

// 맵 데이터 저장 함수
int dup_info(char *str, t_map *map_data)
{
    int status;

    status = 0;
    status = handle_texture_data(str, map_data);
    if (status != 0)
        return (status);
        
    status = handle_color_data(str, map_data);
    if (status != -1)
        return (status);
    
    if (ft_onlyisspace(str) && map_data->exf > 63)
        return (-1);
    
    status = is_right_map(str, map_data);
    if (status == -1)
        return (-1);
    
    return (1);
}

// 맵 데이터 파일 삽입 함수
int insert_data(t_map *map_data, char *map)
{
    char *temp;

    map_data->fd = open(map, O_RDONLY);
    if (map_data->fd < 0)
    {
        printf("Error\n파일을 열 수 없습니다.\n");
        return (0);
    }
    map_data->map = (char **)ft_calloc(sizeof(char *), (map_data->high - 4));
    map_data->map[0] = (char *)ft_calloc(sizeof(char), map_data->map_width + 2);
    ft_memset(map_data->map[0], 'X', map_data->map_width + 1);
    if (map_data->map == NULL)
        return (0);
    
    while ((temp = get_next_line(map_data->fd)))
    {
        if(dup_info(temp, map_data) == 1 && map_data->exf^63)
            map_data->exf = (map_data->exf >> 1);
        ft_freenull(&temp);
    }
    map_data->map[map_data->map_height] = ft_calloc(sizeof(char), 
                                                  map_data->map_width + 2);
    ft_memset(map_data->map[map_data->map_height], 'X', map_data->map_width + 1);
    flud_fill(map_data->map);
    close(map_data->fd);
    return (1);
}
