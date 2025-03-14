/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinseo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 21:57:31 by jinseo            #+#    #+#             */
/*   Updated: 2025/03/13 21:57:32 by jinseo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

int main(int argc, char **argv)
{
    t_data data;

    if (check_args(argc, argv, &data) == 0)
        return (1);

    init_mlx(&data);
    init_cub3d_program(&data);
    find_obj(&data);
    init_player_direction(&data);
    
    init_window(&data);
    init_image(&data);
    load_textures(&data);
    
    data.view_mode = 1;
    
    update_view(&data);
    setup_events(&data);
    
    mlx_loop(data.mlx);
    
    return (0);
}

int check_args(int argc, char **argv, t_data *data)
{
    if (argc != 2)
    {
        printf("Error check argument count\n");
        return (0);
    }

    if (!map_parsing(argv[1], data))
    {
        printf("Error\n맵 파싱에 실패했습니다.\n");
        return (0);
    }
    
    return (1);
}

void init_mlx(t_data *data)
{
    data->mlx = mlx_init();
    if (!data->mlx)
    {
        printf("Mlx Init Failed ");
        exit(1);
    }
}

void init_window(t_data *data)
{
    data->width = 800;
    data->height = 600;
    
    data->win = mlx_new_window(data->mlx, data->width, data->height, "Cub3D");
    if (!data->win)
    {
        printf("Error\n윈도우 생성에 실패했습니다.\n");
        free(data->mlx);
        exit(1);
    }
}

void init_image(t_data *data)
{
    data->img.img = mlx_new_image(data->mlx, data->width, data->height);
    data->img.buffer = mlx_get_data_addr(data->img.img, &data->img.pixel_bits,
            &data->img.line_bytes, &data->img.endian);
}
