#include "../include/cub3d.h"

// 3D 뷰에서 천장과 바닥을 그리는 함수
void draw_floor_ceiling(t_data *data)
{
    int x;
    int y;
    char *dst;

    // 천장 - 맵에서 설정한 천장 색상 사용
    for (y = 0; y < data->height / 2; y++)
    {
        for (x = 0; x < data->width; x++)
        {
            dst = data->img.buffer + (y * data->img.line_bytes) + (x * (data->img.pixel_bits / 8));
            *(unsigned int *)dst = data->map.C; // 천장 색상 사용
        }
    }

    // 바닥 - 맵에서 설정한 바닥 색상 사용
    for (y = data->height / 2; y < data->height; y++)
    {
        for (x = 0; x < data->width; x++)
        {
            dst = data->img.buffer + (y * data->img.line_bytes) + (x * (data->img.pixel_bits / 8));
            *(unsigned int *)dst = data->map.F; // 바닥 색상 사용
        }
    }
}

// 3D 뷰에서 레이캐스팅 결과에 따라 벽을 그리는 함수
void draw_walls_3d(t_data *data)
{
    double ray_angle;
    int x;
    t_ray ray;
    int wall_height;
    int draw_start;
    int draw_end;
    int y;
    char *dst;
    int wall_color;

    // 시야각의 왼쪽 끝부터 오른쪽 끝까지 레이 발사
    ray_angle = data->cor.dir - (data->fov / 2);
    
    for (x = 0; x < data->width; x++)
    {
        // 단일 레이 발사
        ray = cast_single_ray(data, ray_angle);

        // 어안 효과 보정 (fishbowl effect correction)
        double correction = cos(ray_angle - data->cor.dir);
        double corrected_dist = ray.perp_wall_dist * correction;

        // 거리가 0이 되지 않도록 방지 (나눗셈 오류 방지)
        corrected_dist = (corrected_dist < 0.1) ? 0.1 : corrected_dist;
        
        // 벽의 높이 계산 (거리에 반비례)
        // 화면 비율에 맞게 상수 조정 (0.5는 화면 크기에 따라 조정 가능)
        wall_height = (int)(data->height / corrected_dist * 0.5);

        // 그릴 벽의 시작점과 끝점 계산
        draw_start = -wall_height / 2 + data->height / 2;
        if (draw_start < 0)
            draw_start = 0;
        draw_end = wall_height / 2 + data->height / 2;
        if (draw_end >= data->height)
            draw_end = data->height - 1;

        // 벽의 색상 결정 (방향에 따라 다른 색상)
        if (ray.side == 0) // 동서 방향 벽
        {
            if (ray.dir_x > 0)
                wall_color = 0xFF0000;  // 동쪽(빨강)
            else
                wall_color = 0x00FF00;  // 서쪽(초록)
            
            // 그림자 효과: 세로 벽은 약간 어둡게 표시 (선택사항)
            wall_color = (wall_color & 0xFEFEFE) >> 1;
        }
        else // 남북 방향 벽
        {
            if (ray.dir_y > 0)
                wall_color = 0x0000FF;  // 남쪽(파랑)
            else
                wall_color = 0xFFFF00;  // 북쪽(노랑)
        }

        // 벽 그리기 - 화면 범위 검사 추가
        for (y = draw_start; y < draw_end; y++)
        {
            if (y >= 0 && y < data->height && x >= 0 && x < data->width)
            {
                dst = data->img.buffer + (y * data->img.line_bytes) + (x * (data->img.pixel_bits / 8));
                *(unsigned int *)dst = wall_color;
            }
        }

        // 다음 레이 각도 계산
        ray_angle += data->fov / data->width;
    }
}

double get_wall_x(t_data *data, t_ray *ray)
{
    double wall_x;
    
    if (ray->side == 0)
        wall_x = data->cor.y + ray->perp_wall_dist * ray->dir_y;
    else
        wall_x = data->cor.x + ray->perp_wall_dist * ray->dir_x;
    
    wall_x -= floor(wall_x);
    
    return wall_x;
}


// 3D 렌더링 메인 함수
void	render_3d(t_data *data)
{
	// 천장과 바닥 먼저 그리기
	draw_floor_ceiling(data);
	
	// 그 다음 벽 그리기
	draw_walls_3d(data);
	
	// 최종 이미지를 윈도우에 표시
	mlx_put_image_to_window(data->mlx, data->win, data->img.img, 0, 0);
}