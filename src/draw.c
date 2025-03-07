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
    int i
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
    int x
    int y;
    int color;

    for (y = 0; y < data->map.map_height; y++)
    {
        for (x = 0; x < data->map.map_width; x++)
        {
            if (data->map.map[y][x] == '1')
                color = 0xFFFFFF;  // 벽
            else if (data->map.map[y][x] == '0')
                color = 0x000000;  // 바닥
            else if (ft_isinstr(data->map.map[y][x], "NSWE"))
                color = 0xFF0000;  // 플레이어 위치
            else
                continue;

            draw_square_to_image(data, x, y, color);
        }
    }
    mlx_put_image_to_window(data->mlx, data->win, data->img.img, 0, 0);
}
