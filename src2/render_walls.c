/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_walls.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinseo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 21:58:18 by jinseo            #+#    #+#             */
/*   Updated: 2025/03/13 21:58:20 by jinseo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"
/*
void draw_walls_3d(t_data *data)
{
    double ray_angle;
    int x;
    t_ray ray;
    int wall_height;
    double corrected_dist;
    
    ray_angle = data->cor.dir - (data->fov / 2);//왼쪽 30도 부터 광선을 쏜다.
    x = 0;
    while (x < data->width) //화면의 왼쪽부터 쏘게 됨
    {
        ray = cast_single_ray(data, ray_angle); // 화면에 광선을 쏘는 함수
        corrected_dist = ray.perp_wall_dist; // 벽까지의 거리를 저장하는 함수
        correct_ray_distance(&corrected_dist, ray_angle, data); // 화면 왜곡을 방지하기 위해 거리를 보정하는 함수
        wall_height = calculate_wall_height(data, corrected_dist); // 벽 높이를 계산하는 함수
        draw_textured_wall(data, x, ray, wall_height); // 벽을 그리는 함수
        ray_angle += data->fov / data->width; // 다음 광선 각도로 이동한다.
        x++;
    }
}
*/

void draw_walls_3d(t_data *data)
{
    int x; // 화면의 x 좌표를 위한 변수
    double ray_angle; // 광선이 발사되는 각도
    double step;
    t_ray ray;

    step = data->fov / data->width;
    ray_angle = data->cor.dir - (data->fov / 2); // 왼쪽 30도부터 광선을 쏜다.
    x = 0; // 화면의 왼쪽부터 광선을 쏜다.
    while (x < data->width) // 화면의 모든 픽셀을 대상으로 광선을 쏜다.
    {
        ray = cast_single_ray(data, ray_angle); // 광선을 쏘고, 벽과의 충돌을 계산하는 함수
        process_wall_slice(data, x, &ray); // 계산된 데이터를 바탕으로 벽을 그리는 함수
        ray_angle += step; // 각 광선의 간격을 설정한 후에 다음 광선 각도로 이동, 그러니까 각 픽셀 하나당 얼마만큼의 각도를 돌려서 광선을 쏠지
        x++; // 다음 x 좌표로 이동한다.
    }
}

void process_wall_slice(t_data *data, int x, t_ray *ray) // 최적화용 추가함수 입니다 25줄 맞추기용
{
    double corrected_dist; // 보정된 벽까지의 거리
    int wall_height; // 계산된 벽의 높이

    corrected_dist = correct_ray_distance(ray->perp_wall_dist, ray->angle, data); // 화면 왜곡(Fish Eye) 보정
    wall_height = calculate_wall_height(data, corrected_dist); //  벽까지 거리가 가까우면 크고, 멀면 작게 벽 높이를 계산하는 함수
    draw_textured_wall(data, x, *ray, wall_height); // 벽을 그리는 함수
}

void correct_ray_distance(double *corrected_dist, double ray_angle, t_data *data)
{
    double correction; //거리 보정을 위한 변수
    
    correction = cos(ray_angle - data->cor.dir); //거리 왜곡을 보정하는 값 계산
    *corrected_dist = *corrected_dist * correction; //실제 벽 거리를 보정
    
    if (*corrected_dist < 0.1)
        *corrected_dist = 0.1
}

int calculate_wall_height(t_data *data, double corrected_dist)
{
    int wall_height;
    
    wall_height = (int)(data->height / corrected_dist * 0.5);
    return (wall_height); //보정된 거리로 리턴
}

void render_3d(t_data *data)
{
    draw_floor_ceiling(data);
    draw_walls_3d(data);//이부분
    mlx_put_image_to_window(data->mlx, data->win, data->img.img, 0, 0);
}

/* ------------------------------------------------------------------------------------
전체적인 실행에 따른 구동방식 설명
remder3d 함수에 들어오면서 그림을 그리기 시작하는데, 가장 먼저 바닥과 천장을 그리게 되고, 내려와서 벽을 그리게 되는데
draw_walls_3d(data) 이 함수를 통해서 이루어진다.
이후에 cast_single_ray 여기서 광선을 하나하나 씩 소게 되고,
init_ray_direction() 광선의 방향을 정하게 되고
init_ray_map_pos() 어디서 쏘는지 위치를 설정하고
calculate_delta_dist() 광선이 얼마나 이동하는지 계산을 한다
calculate_step_side_dist 그리고 어떤 방향으로 얼마만큼 움직일건지 설정을하고
perform_dda() dda 알고리즘을 통해 브레젠햄 직선 방정식으로 광선을 이동시킨다
calculate_wall_distance() 나온 것을 바탕으로 벽까지의 거리를 계산하고
process_wall_slice() 보정, 높이 계산을 마무리 하면서 벽 그리기가 끝이난다.

지금 바뀐 함수마다 추가 함수는 추가함수라고 적어놨고, 아직 헤더파일은 바꾸지 않았습니다. 일단 올리고, 오늘 클러스터가서 그건 수정 좀 하겠습니다.

*/