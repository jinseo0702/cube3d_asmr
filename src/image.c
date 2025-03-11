#include "cub3d.h"

// 이미지 파일 텍스처 저장 함수 2025.03.11

/*
void draw_wall_with_texture(t_data *data, int x, t_ray *ray, mlx_texture_t *texture)
{
    int wall_height = (int)(data->height / (ray->perp_wall_dist));
    int draw_start = -wall_height / 2 + data->height / 2;
    int draw_end = wall_height / 2 + data->height / 2;
    int tex_x = get_wall_texture_x(data, ray);
    int tex_y;
    int color;

    for (int y = draw_start; y < draw_end; y++)
    {
        tex_y = ((y - draw_start) * texture->height) / wall_height;
        color = *(unsigned int *)(texture->pixels + (tex_y * texture->width + tex_x) * 4);
        mlx_pixel_put(data->mlx, data->win, x, y, color);
    }
}

벽 텍스처가 만들어졌을 경우 예시 코드 벽이 만들어지고나서 텍스쳐를 입혀야 한다.

void render_3d_walls(t_data *data)
{
    double ray_angle = data->cor.dir - (data->fov / 2);
    t_ray ray;
    mlx_texture_t *texture;

    for (int x = 0; x < data->width; x++)
    {
        ray = cast_single_ray(data, ray_angle);

        if (ray.side == 0 && ray.dir_x < 0)
            texture = data->west;
        else if (ray.side == 0 && ray.dir_x > 0)
            texture = data->east;
        else if (ray.side == 1 && ray.dir_y < 0)
            texture = data->north;
        else
            texture = data->south;

        draw_wall_with_texture(data, x, &ray, texture);
        ray_angle += data->fov / data->width;
    }
}

천장과 바닥 3d화를 했을 시에 색칠한 예시

void draw_floor_and_ceiling(t_data *data)
{
    int x, y;

    for (y = 0; y < data->height / 2; y++) // 천장
    {
        for (x = 0; x < data->width; x++)
            mlx_pixel_put(data->mlx, data->win, x, y, data->map.ceiling_color);
    }

    for (y = data->height / 2; y < data->height; y++) // 바닥
    {
        for (x = 0; x < data->width; x++)
            mlx_pixel_put(data->mlx, data->win, x, y, data->map.floor_color);
    }
}


*/


void load_textures(t_data *data)
{
    data->north = mlx_xpm_file_to_image(data->mlx, data->map.NO, &data->tex_width, &data->tex_height);
    data->south = mlx_xpm_file_to_image(data->mlx, data->map.SO, &data->tex_width, &data->tex_height);
    data->west = mlx_xpm_file_to_image(data->mlx, data->map.WE, &data->tex_width, &data->tex_height);
    data->east = mlx_xpm_file_to_image(data->mlx, data->map.EA, &data->tex_width, &data->tex_height);

    if (!data->north || !data->south || !data->west || !data->east)
        handle_error("Error: Failed to load textures");
}