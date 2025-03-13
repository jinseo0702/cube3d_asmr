/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flood_fill.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinseo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 21:57:23 by jinseo            #+#    #+#             */
/*   Updated: 2025/03/13 21:59:25 by jinseo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void solve_Dfs(char **map, int x, int y, int *status)
{
    if (*status == -1)
        return ;
    if (map[x][y] == 'X')
    {
        *status = -1;
        return ;
    }
    else if(map[x][y] != '0')
        return ;
    map[x][y] += 2;
    solve_Dfs(map, (x - 1), y, status);
    solve_Dfs(map, (x + 1), y, status);
    solve_Dfs(map, x, (y - 1), status);
    solve_Dfs(map, x, (y + 1), status);
}

int flud_fill(char **map)
{
    int status;
    int x;
    int y;

    status = 0;
    x = 0;
    while (map[x])
    {
        if (status == -1)
        {
            printf("Error\n맵이 닫혀있지 않습니다.\n");
            return (-1);
        }
        y = 0;
        while (map[x][y])
        {
            if (map[x][y] == '0')
                solve_Dfs(map, x, y, &status);
            ++y;
        }
        ++x;
    }
    return (1);
}

// 배열 길이 구하는 유틸리티 함수
int ft_arraylen(char **array)
{
    int i;
    
    i = 0;
    while (array[i])
        i++;
    return (i);
}

// 배열 메모리 해제 함수
void ft_free_array(char **array)
{
    int i;
    
    i = 0;
    while (array[i])
    {
        free(array[i]);
        i++;
    }
    free(array);
}

// 초기화 함수
void init_cub3d_program(t_data *data)
{
    data->width = 2000;
    data->height = 2000;
    data->img.width = data->width;
    data->img.height = data->height;
    data->x_offset = 500;
    data->y_offset = 500;
    data->ray_len = 10.0;
    data->ray_count = 120;
    data->fov = M_PI / 3;
}
