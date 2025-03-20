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

void solve_Dfs(char **map, int x, int y)
{
    if (map[x][y] == 'X')
    {
        printf("Error  Map style is not Good!");
        exit (1);
    }
    else if(map[x][y] != '0')
        return ;
    map[x][y] += 2;
    solve_Dfs(map, (x - 1), y);
    solve_Dfs(map, (x + 1), y);
    solve_Dfs(map, x, (y - 1));
    solve_Dfs(map, x, (y + 1));
}

int flood_fill(char **map)
{
    int x;
    int y;

    x = -1;
    while (map[++x])
    {
        y = -1;
        while (map[x][++y])
            if (map[x][y] == '0')
                solve_Dfs(map, x, y);
    }
    return (TRUE);
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
    data->width = 1920;
    data->height = 1080;
    data->img.width = data->width;
    data->img.height = data->height;
    data->fov = M_PI / 3;
    data->status = 0;
}
