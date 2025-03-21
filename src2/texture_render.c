/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_render.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinseo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 21:58:21 by jinseo            #+#    #+#             */
/*   Updated: 2025/03/13 21:58:22 by jinseo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void draw_textured_wall(t_data *data, int x, t_ray ray, int wall_height)
{
    t_wall_tex tex;
    int draw_start;
    int draw_end;
    t_allimg *tex_img;
    
    calculate_wall_bounds(wall_height, data, &draw_start, &draw_end); // 벽이 화면에서 차지하는 범위를 계산
    tex_img = select_texture(ray, data);    // 광선이 충돌한 벽의 텍스처 선택
    
    tex.buffer = tex_img->buffer;
    tex.line_bytes = tex_img->line_bytes;
    tex.pixel_bits = tex_img->pixel_bits;
    tex.tex_x = get_tex_x(data, &ray);
    tex.height = wall_height;
    
    draw_wall_line(data, x, draw_start, draw_end, tex);
}
/*
void draw_wall_line(t_data *data, int x, int start, int end, t_wall_tex tex)
{
    int y;
    int tex_y;
    double step;
    double tex_pos;
    char *dst;
    char *src;
    int color;
    
    step = (double)TEX_HEIGHT / tex.height;
    tex_pos = (start - data->height / 2 + tex.height / 2) * step;
    
    y = start;
    while (y < end)
    {
        tex_y = (int)tex_pos & (TEX_HEIGHT - 1); // 텍스처의 Y 좌표를 계산
        tex_pos += step;
        
        src = tex.buffer + (tex_y * tex.line_bytes + tex.tex_x * (tex.pixel_bits / 8));
        color = *(unsigned int *)src;
        
        if (y >= 0 && y < data->height && x >= 0 && x < data->width)
        {
            dst = data->img.buffer + (y * data->img.line_bytes + x * (data->img.pixel_bits / 8));
            *(unsigned int *)dst = color; // 색상을 픽셀에 적용
        }
        y++;
    }
}
*/

void draw_wall_line(t_data *data, int x, int start, int end, t_wall_tex tex)
{
    int y;  // 화면에 그릴 현재 y 좌표
    double step;    // 텍스처의 y축에서 한 줄 그릴 때마다 얼마나 이동할지
    double tex_pos; // 텍스처에서 현재 그릴 y 위치
    char *dst;  // 화면에 색을 넣을 위치 (버퍼 주소)
    
    step = (double)TEX_HEIGHT / tex.height;
    // 벽의 실제 높이와 텍스처 높이를 이용해서 한 줄당 이동할 간격을 구해
    tex_pos = (start - data->height / 2 + tex.height / 2) * step;
    // 벽을 화면에 중앙 정렬하기 위해 텍스처 시작 위치를 계산해

    y = start;
    // 위에서 계산한 y 시작점부터 아래로 내려가며 그림
    
    while (y < end) // y가 벽의 끝지점까지 도달할 때까지 반복
    {
        int color = get_texture_color(&tex, tex_pos); // 텍스처 색상 가져오기
        tex_pos += step;
         // 다음 줄 그릴 때 사용할 텍스처 y 좌표로 이동

        if (y >= 0 && y < data->height && x >= 0 && x < data->width)
        {
            dst = data->img.buffer + (y * data->img.line_bytes + x * (data->img.pixel_bits / 8));
            // 화면 버퍼에서 실제로 색상을 넣을 위치를 계산해서
            *(unsigned int *)dst = color; // 픽셀에 색상 적용하고 화면에 표시
        }
        y++; // 다음줄로 내려가기
    }
}

int get_texture_color(t_wall_tex *tex, double tex_pos) // 최적화 하면서 추가 된 함수
{
    int tex_y;  // 텍스처 이미지의 y 좌표
    char *src;  // 텍스처에서 색상 정보가 저장된 위치
    int color;  // 최종적으로 꺼내올 색상 값

    tex_y = (int)tex_pos & (TEX_HEIGHT - 1);     // 소수점 tex_pos를 정수 tex_y로 변환하고, 텍스처 높이를 넘지 않도록 마스킹 처리
    src = tex->buffer + (tex_y * tex->line_bytes + tex->tex_x * (tex->pixel_bits / 8));    // 텍스처 메모리에서 정확히 tex_x, tex_y 위치에 해당하는 주소를 계산
    color = *(unsigned int *)src; // 텍스처 색상 추출

    return color;
}


void init_textures(t_data *data)
{
    int width;
    int height;

    data->tex_n.img = mlx_xpm_file_to_image(data->mlx, data->map.NO, &width, &height);
    data->tex_s.img = mlx_xpm_file_to_image(data->mlx, data->map.SO, &width, &height);
    data->tex_w.img  = mlx_xpm_file_to_image(data->mlx, data->map.WE, &width, &height);
    data->tex_e.img = mlx_xpm_file_to_image(data->mlx, data->map.EA, &width, &height);
    if (!data->tex_n.img || !data->tex_s.img || !data->tex_w.img || !data->tex_e.img)
    {
        printf("image load fail!! Check again!\n");
        exit(1);
    }
    data->tex_n.buffer = mlx_get_data_addr(data->tex_n.img, 
        &data->tex_n.pixel_bits, &data->tex_n.line_bytes, &data->tex_n.endian);
    data->tex_s.buffer = mlx_get_data_addr(data->tex_s.img, 
        &data->tex_s.pixel_bits, &data->tex_s.line_bytes, &data->tex_s.endian);
    data->tex_w.buffer = mlx_get_data_addr(data->tex_w.img, 
        &data->tex_w.pixel_bits, &data->tex_w.line_bytes, &data->tex_w.endian);
    data->tex_e.buffer = mlx_get_data_addr(data->tex_e.img, 
        &data->tex_e.pixel_bits, &data->tex_e.line_bytes, &data->tex_e.endian);
}
