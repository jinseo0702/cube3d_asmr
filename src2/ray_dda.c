/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_dda.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinseo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 21:57:58 by jinseo            #+#    #+#             */
/*   Updated: 2025/03/13 21:58:08 by jinseo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"
/*
int check_map_bounds(t_ray *ray, t_data *game)
{
    if (ray->map_y < 0 || ray->map_y >= game->map.map_height || 
        ray->map_x < 0 || ray->map_x >= game->map.map_width)
    {
        ray->hit = 1;
        ray->perp_wall_dist = 20.0;
        return (1);
    }
    return (0);
}

int check_wall_hit(t_ray *ray, t_data *game)
{
    if (game->map.map[ray->map_y][ray->map_x] == '1' || 
        game->map.map[ray->map_y][ray->map_x] == 'X')
    {
        ray->hit = 1;
        return (1);
    }
    return (0);
}
*/
/*
void perform_dda(t_ray *ray, t_data *game)
{
    ray->hit = 0;
    while (ray->hit == 0) //벽을 만날 때까지 한 칸 씩 이동하는 브레젠햄 알고리즘
    {
        if (ray->side_dist_x < ray->side_dist_y) // x방향이 더 가까울때
        {
            ray->side_dist_x += ray->delta_dist_x;
            ray->map_x += ray->step_x;
            ray->side = 0;
        }
        else // y방향이 더 가까울 때
        {
            ray->side_dist_y += ray->delta_dist_y;
            ray->map_y += ray->step_y;
            ray->side = 1;
        }
        
        if (check_map_bounds(ray, game) == 1) //맵 경계를 넘게되면 종료
            break;
        check_wall_hit(ray, game); // 벽을 만났는지 확인
    }
}
*/

void perform_dda(t_ray *ray, t_data *game)
{
    char **map = game->map.map; // 게임 맵 데이터를 가져옴
    int width = game->map.map_width; // 맵의 가로 길이
    int height = game->map.map_height; // 맵의 세로 길이

    ray->hit = 0; // 벽 충돌 여부 초기화
    while (!ray->hit) // 벽을 만날 때까지 반복
    {
        step_to_next_grid(ray); // 다음 그리드로 이동
        if (check_wall_collision(ray, map, width, height)) // 벽에 부딪혔는지 검사
            ray->hit = 1; // 벽을 만났으면 종료
    }
}

int check_wall_collision(t_ray *ray, char **map, int width, int height)
{
    // 맵 범위를 벗어난 경우
    if (ray->map_x < 0 || ray->map_x >= width ||
        ray->map_y < 0 || ray->map_y >= height)
    {
        ray->perp_wall_dist = 20.0; // 너무 멀리 벗어난 광선은 먼 거리로 간주
        return (1);
    }
    // 벽에 부딪혔는지 확인
    if (map[ray->map_y][ray->map_x] == '1' || 
        map[ray->map_y][ray->map_x] == 'X')
    {
        return (1);
    }

    return (0);
}

void step_to_next_grid(t_ray *ray)
{
    if (ray->side_dist_x < ray->side_dist_y) // X축 방향이 더 가까우면
    {
        ray->side_dist_x += ray->delta_dist_x; // X 방향 이동 거리 업데이트
        ray->map_x += ray->step_x; // X 좌표 업데이트
        ray->side = 0; // X 방향 충돌 기록
    }
    else // Y축 방향이 더 가까우면
    {
        ray->side_dist_y += ray->delta_dist_y; // Y 방향 이동 거리 업데이트
        ray->map_y += ray->step_y; // Y 좌표 업데이트
        ray->side = 1; // Y 방향 충돌 기록
    }
}

void calculate_wall_distance(t_ray *ray, t_data *game)
{
    if (ray->side == 0)
        ray->perp_wall_dist = fabs((ray->map_x - game->cor.x 
            + (1 - ray->step_x) / 2) / ray->dir_x);
    else
        ray->perp_wall_dist = fabs((ray->map_y - game->cor.y 
            + (1 - ray->step_y) / 2) / ray->dir_y);
}
