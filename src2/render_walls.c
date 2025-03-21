/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_walls.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinseo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 21:58:18 by jinseo            #+#    #+#             */
/*   Updated: 2025/03/21 21:59:50 by jinseo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void draw_walls_3d(t_data *data)
{
	int x; 
	double ray_angle; 
	double step;
	t_ray ray;

	step = data->fov / data->width;
	ray_angle = data->cor.dir - (data->fov / 2); 
	x = 0; 
	while (x < data->width) 
	{
		ray = cast_single_ray(data, ray_angle); 
		process_wall_slice(data, x, &ray, ray_angle); 
		ray_angle += step; 
		x++; 
	}
}

void process_wall_slice(t_data *data, int x, t_ray *ray, double ray_angle)
{
	double corrected_dist;
	int wall_height;

	corrected_dist = correct_ray_distance(ray->perp_wall_dist, ray_angle, data);
	wall_height = calculate_wall_height(data, corrected_dist);
	draw_textured_wall(data, x, *ray, wall_height);
}

double correct_ray_distance(double corrected_dist, double ray_angle, t_data *data)
{
	double correction; 

	correction = cos(ray_angle - data->cor.dir); 
	corrected_dist = corrected_dist * correction; 

	if (corrected_dist < 0.1)
		return (corrected_dist = 0.1);
	else
		return corrected_dist;
}

int calculate_wall_height(t_data *data, double corrected_dist)
{
	int wall_height;

	wall_height = (int)(data->height / corrected_dist * 0.5);
	return (wall_height); 
}

void render_3d(t_data *data)
{
	draw_floor_ceiling(data);
	draw_walls_3d(data);
	mlx_put_image_to_window(data->mlx, data->win, data->img.img, 0, 0);
}

/* ------------------------------------------------------------------------------------
 * 전체적인 실행에 따른 구동방식 설명
 * remder3d 함수에 들어오면서 그림을 그리기 시작하는데, 가장 먼저 바닥과 천장을 그리게 되고, 내려와서 벽을 그리게 되는데
 * draw_walls_3d(data) 이 함수를 통해서 이루어진다.
 * 이후에 cast_single_ray 여기서 광선을 하나하나 씩 소게 되고,
 * init_ray_direction() 광선의 방향을 정하게 되고
 * init_ray_map_pos() 어디서 쏘는지 위치를 설정하고
 * calculate_delta_dist() 광선이 얼마나 이동하는지 계산을 한다
 * calculate_step_side_dist 그리고 어떤 방향으로 얼마만큼 움직일건지 설정을하고
 * perform_dda() dda 알고리즘을 통해 브레젠햄 직선 방정식으로 광선을 이동시킨다
 * calculate_wall_distance() 나온 것을 바탕으로 벽까지의 거리를 계산하고
 * process_wall_slice() 보정, 높이 계산을 마무리 하면서 벽 그리기가 끝이난다.
 *
 * 지금 바뀐 함수마다 추가 함수는 추가함수라고 적어놨고, 아직 헤더파일은 바꾸지 않았습니다. 일단 올리고, 오늘 클러스터가서 그건 수정 좀 하겠습니다.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
