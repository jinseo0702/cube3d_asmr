#include "../include/cub3d.h"
/*
#define TILE_SIZE 32  // 하나의 타일 크기

void draw_square(t_data *data, int x, int y, int color) // 점을 하나씩 찍어서 사각형의 공간을 만들어내는 코드 입니다
{
    int i, j;
    int pixel_x = x * TILE_SIZE;
    int pixel_y = y * TILE_SIZE;

    for (i = 0; i < TILE_SIZE; i++)
    {
        for (j = 0; j < TILE_SIZE; j++)
        {
            mlx_pixel_put(data->mlx, data->win, pixel_x + j, pixel_y + i, color);
        }
    }
}

void draw_map_from_array(t_data *data)
{
    int x, y;
    int color;

    for (y = 0; y < data->map.map_height; y++)
    {
        for (x = 0; x < data->map.map_width; x++)
        {
            if (data->map.map[y][x] == '1')
                color = 0xFFFFFF;  // 흰색 벽
            else if (data->map.map[y][x] == '0')
                color = 0x000000;  // 검은색 바닥
            else if (ft_isinstr(data->map.map[y][x], "NSWE"))
                color = 0xFF0000;  // 플레이어 위치는 빨간색
            else
                continue;

            draw_square(data, x, y, color);
        }
    }
}
*/

#define TILE_SIZE 32

void draw_square_to_image(t_data *data, int x, int y, int color) // 이 코드의 경우에는 이미지 버퍼에 픽셀들을 찍고 한번에 이미지를 찍어내는 코드입니다.
{
    int i;
    int j;
    int pixel_x = x * TILE_SIZE;
    int pixel_y = y * TILE_SIZE;
    char *dst;

    for (i = 0; i < TILE_SIZE; i++)
    {
        for (j = 0; j < TILE_SIZE; j++)
        {
            dst = data->img.buffer + ((pixel_y + i) * data->img.line_bytes) + ((pixel_x + j) * (data->img.pixel_bits / 8));
            *(unsigned int *)dst = color;
        }
    }
}

void draw_map_from_array(t_data *data)
{
    int x;
    int y;
    int color;

    // print_all(&data->map);
    mlx_clear_window(data->mlx, data->win);
    for (y = 0; y < data->map.map_height; y++)
    {
        for (x = 0; x < data->map.map_width; x++)
        {
            if (data->map.map[y][x] == '1')
                color = 0xFFFFFF;  // 벽
            else if (data->map.map[y][x] == '2')
                color = 0x000000;  // 바닥
            else if (ft_isinstr(data->map.map[y][x], "NSWE"))
                color = 0xFF0000;  // 플레이어 위치
            else
                continue;

            draw_square_to_image(data, x, y, color);
        }
    }
    draw_rays(data);
    mlx_put_image_to_window(data->mlx, data->win, data->img.img, 0, 0);
}


// draw.c에 추가하거나 새로운 파일에 만들기
void draw_line_to_image(t_data *data, int x1, int y1, int x2, int y2, int color)
{
    int dx = abs(x2 - x1);
    int dy = -abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = dx + dy;
    int e2;
    char *dst;

    while (1)
    {
        // 이미지 범위 체크
        if (x1 >= 0 && x1 < data->width && y1 >= 0 && y1 < data->height)
        {
            dst = data->img.buffer + (y1 * data->img.line_bytes) + (x1 * (data->img.pixel_bits / 8));
            *(unsigned int *)dst = color;
        }
        
        if (x1 == x2 && y1 == y2)
            break;
            
        e2 = 2 * err;
        if (e2 >= dy)
        {
            if (x1 == x2)
                break;
            err += dy;
            x1 += sx;
        }
        if (e2 <= dx)
        {
            if (y1 == y2)
                break;
            err += dx;
            y1 += sy;
        }
    }
}

void draw_rays(t_data *data)
{
    double ray_angle;
    t_ray ray;
    int start_x, start_y, end_x, end_y;
    
    // // 북쪽 방향 (-M_PI/2) 또는 플레이어가 바라보는 방향
    double player_dir = -M_PI / 2; // 북쪽 방향으로 설정
    
    // // 단일 광선 그리기 (북쪽 방향)
    // ray_angle = player_dir;
    // ray = cast_single_ray(data, ray_angle);
    
    // // 시작점 (플레이어 위치)
    start_x = data->cor.x * TILE_SIZE + TILE_SIZE / 2;
    start_y = data->cor.y * TILE_SIZE + TILE_SIZE / 2;
    
    // // 끝점 (광선이 벽에 닿는 지점)
    double ray_len = ray.perp_wall_dist;
    // end_x = start_x + cos(ray_angle) * ray_len * TILE_SIZE;
    // end_y = start_y + sin(ray_angle) * ray_len * TILE_SIZE;
    
    // // 광선 그리기
    // draw_line_to_image(data, start_x, start_y, end_x, end_y, 0xFF0000); // 빨간색 광선
    
    // 다중 광선 그리기를 위한 코드 (선택적)
    
    // 시야각의 왼쪽 끝부터 오른쪽 끝까지 여러 광선 쏘기
    ray_angle = player_dir - (data->fov / 2);
    for (int i = 0; i < data->ray_count; i++)
    {
        ray = cast_single_ray(data, ray_angle);
        
        // 끝점 계산
        end_x = start_x + cos(ray_angle) * ray.perp_wall_dist * TILE_SIZE;
        end_y = start_y + sin(ray_angle) * ray.perp_wall_dist * TILE_SIZE;
        
        // 광선 그리기
        draw_line_to_image(data, start_x, start_y, end_x, end_y, 0xFFFF33);
        
        // 다음 광선 각도
        ray_angle += data->fov / data->ray_count;
    }
}
