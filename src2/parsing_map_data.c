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
    char *find;

    find = ft_strchr(src, '.');
    *dst = ft_strdup_flag(find, status);
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
        copy_texture_path(&map_data->NO, str, &status);
    else if (ft_strncmp(str, "SO ", 3) == 0)
        copy_texture_path(&map_data->SO, str, &status);
    else if (ft_strncmp(str, "WE ", 3) == 0)
        copy_texture_path(&map_data->WE, str, &status);
    else if (ft_strncmp(str, "EA ", 3) == 0)
        copy_texture_path(&map_data->EA, str, &status);
    else if (ft_strncmp(str, "F ", 2) == 0)
        map_data->F = parse_color(str);
    else if (ft_strncmp(str, "C ", 2) == 0)
        map_data->C = parse_color(str);
    return (status);
}

// 맵 데이터 저장 함수
int dup_info(char *str, t_map *map_data, t_data *data)
{
    if (ft_onlyisspace(str) && map_data->exf > 63)
        return (-1);
    data->status += handle_texture_data(str, map_data);
    if(map_data->exf <= 127)
        is_right_map(str, map_data, data);
    return (1);
}

// 맵 데이터 파일 삽입 함수
int insert_data(t_map *map_data, char *map, t_data *data)
{
    char *temp;

    map_data->fd = open(map, O_RDONLY);
    if (map_data->fd < 0)
    {
        printf("Error Failed opne File.\n");//Free 시켜주고 확인 을 해야합니다.
        exit(0);
    }
    map_data->map = (char **)ft_calloc(sizeof(char *), (map_data->high));
    map_data->map[0] = (char *)ft_calloc(sizeof(char), map_data->map_width + 2);
    ft_memset(map_data->map[0], 'X', map_data->map_width + 1);
    if (map_data->map == NULL)
        exit(0);//free 시켜주는 함수 만들어 줘야합니다.
    while ((temp = get_next_line(map_data->fd)))
    {
        if(dup_info(temp, map_data, data) == 1 && map_data->exf^63)
            map_data->exf = (map_data->exf >> 1);
        ft_freenull(&temp);
    }
    map_data->map[map_data->map_height] = ft_calloc(sizeof(char), 
                                                  map_data->map_width + 2);
    ft_memset(map_data->map[map_data->map_height], 'X', map_data->map_width + 1);
    flud_fill(map_data->map);
    print_all(map_data);
    close(map_data->fd);
    return (1);
}
