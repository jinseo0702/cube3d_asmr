/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_casting.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinseo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 21:57:56 by jinseo            #+#    #+#             */
/*   Updated: 2025/03/13 21:57:58 by jinseo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

t_ray cast_single_ray(t_data *game, double angle)
{
    t_ray ray;
    
    init_ray_direction(&ray, angle); //광선의 방향을 설정한다
    init_ray_map_pos(&ray, game); // 현재 플레이어의 위치에서 광선이 시작할 맵 좌표를 구한다
    calculate_delta_dist(&ray); // x, y 방향으로 광선이 한 칸 이동할 때 얼마나 거리 차이가 나는지 계산한다
    //calculate_step_side_dist_x(&ray, game); // x 방향 거리를 계산한다
    //calculate_step_side_dist_y(&ray, game); // y 방향 거리를 계산한다
    calculate_step_side_dist(&ray, game);
    // ㄴ> 광선이 어떤 방향으로 움직이고, 처음으로 어떤 벽 라인을 만날지, 그리고 x,y 방향으로 광선이 이동할 때, 얼마나 거리차이가 나는지 계산한다. 앞에 두 주석처리한 함수를 하나로 묶음
    perform_dda(&ray, game); // 벽에 부딪힐 때까지 한 칸씩 반복해서 이동한다..
    calculate_wall_distance(&ray, game); // 최종 거리를 계산한다
    return (ray);
}

void init_ray_direction(t_ray *ray, double angle)
{
    ray->dir_x = cos(angle);
    ray->dir_y = sin(angle);
}

void init_ray_map_pos(t_ray *ray, t_data *game)
{
    ray->map_x = (int)game->cor.x;
    ray->map_y = (int)game->cor.y;
}

void calculate_delta_dist(t_ray *ray)
{
    if (ray->dir_x == 0)
        ray->delta_dist_x = 1e30;
    else
        ray->delta_dist_x = fabs(1 / ray->dir_x);
    if (ray->dir_y == 0)
        ray->delta_dist_y = 1e30;
    else
        ray->delta_dist_y = fabs(1 / ray->dir_y);
}

void calculate_step_side_dist(t_ray *ray, t_data *game) // 추가함수 입니다
{
    calculate_step_x(ray, game); // X 방향 이동 거리를 계산하는 함수
    calculate_step_y(ray, game); // Y 방향 이동 거리를 계산하는 함수
}