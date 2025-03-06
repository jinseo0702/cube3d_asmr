#include "../include/cub3d.h"
#define SCALE 30;

void put_pixel_to_image(t_allimg *img, int x, int y, int color)
{
    char *dst;

    if (x < 0 || x >= img->width || y < 0 || y >= img->height)
        return;
    dst = img->buffer + (y * img->line_bytes + x * (img->pixel_bits / 8));
    dst[0] = color & 0xFF;
    dst[1] = (color >> 8) & 0xFF;
    dst[2] = (color >> 16) & 0xFF;
    dst[3] = 0xFF;
}

void init_draw_info(t_draw_func *draw, t_two_coordi_node p1, t_two_coordi_node p2)
{
    draw->dx = abs(p2.x - p1.x);
    draw->dy = abs(p2.y - p1.y);
    draw->err = draw->dx - draw->dy;

    if (p1.x < p2.x)
        draw->sx = 1;
    else
        draw->sx = -1;

    if (p1.y < p2.y)
        draw->sy = 1;
    else
        draw->sy = -1;
}


void draw_line(t_two_coordi_node p1, t_two_coordi_node p2, t_data *data, int color)
{
    t_draw_func draw;

    init_draw_info(&draw, p1, p2);
    while (1)
    {
        put_pixel_to_image(&data->img, p1.x + data->x_offset, p1.y + data->y_offset, color);
        if (p1.x == p2.x && p1.y == p2.y)
            break;
        draw.e2 = draw.err * 2;
        if (draw.e2 > -draw.dy)
	    {
            draw.err -= draw.dy;
            p1.x += draw.sx;
        }
        if (draw.e2 < draw.dx)
	    {
            draw.err += draw.dx;
            p1.y += draw.sy;
        }
    }
}
/*
void draw_map_from_array(t_data *data)
{
    int x;
    int y;
    t_two_coordi_node p1;
    t_two_coordi_node p2;

    y = 0;
    while (y < data->map.map_height)
    {
        x = 0;
        while (x < data->map.map_width)
        {
            p1.x = x * SCALE;
            p1.y = y * SCALE;
            if (x + 1 < data->map.map_width)
            {
                p2.x = (x + 1) * SCALE;
                p2.y = y * SCALE;
                draw_line(p1, p2, data, 0xFFFFFF);
            }

            if (y + 1 < data->map.map_height)
            {
                p2.x = x * SCALE;
                p2.y = (y + 1) * SCALE;
                draw_line(p1, p2, data, 0xFFFFFF);
            }
            x++;
        }
        y++;
    }
}
*/                //이 주석 함수들은 그냥 모든 맵의 띄워쓰기 숫자 다 미포함하고 공간구현입니다
int is_valid_map_char(char c)
{
    return (c == '1' || c == '0' || c == 'N' || c == 'E' || c == 'S' || c == 'W');
}

void draw_map_from_array(t_data *data)
{
    int x, y;
    t_two_coordi_node p1, p2;

    y = 0;
    while (y < data->map.map_height)
    {
        x = 0;
        while (x < data->map.map_width)
        {
            if (is_valid_map_char(data->map.map[y][x]))
            {
                p1.x = x * SCALE;
                p1.y = y * SCALE;

                // 오른쪽으로 선 그리는겁니다
                if (x + 1 < data->map.map_width && is_valid_map_char(data->map.map[y][x + 1]))
                {
                    p2.x = (x + 1) * SCALE;
                    p2.y = y * SCALE;
                    draw_line(p1, p2, data, 0xFFFFFF);
                }

                // 아래로 선 그리는겁니다
                if (y + 1 < data->map.map_height && is_valid_map_char(data->map.map[y + 1][x]))
                {
                    p2.x = x * SCALE;
                    p2.y = (y + 1) * SCALE;
                    draw_line(p1, p2, data, 0xFFFFFF);
                }
            }
            x++;
        }
        y++;
    }
}       //현재 이 함수는 오류가 있습니다. 비슷하게 나오는데 2칸씩 잡아먹을때도 있고, 이상하게 선이 그여지기도 하는데 아직 이유를 모르겠습니다.
