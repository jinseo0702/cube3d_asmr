#include "../include/cub3d.h"

// 생각해보아야할 것
//기본적으로 argv에 .cub파일만 받아오는지

int	ft_key_handling(int keycode, t_data *data)  //이건 esc 컨트롤
{
	if (keycode == KEY_ESCAPE)
	{
		mlx_destroy_image(data->mlx, data->img.img);
		mlx_destroy_window(data->mlx, data->win);
		mlx_destroy_display(data->mlx);
		free(data->mlx);
		exit(0);
	}
	else if(keycode ==  KEY_UP || keycode == KEY_W)
	{
		data->map.map[data->cor.y][data->cor.x] = '2';
		data->cor.y -= 1;
		data->map.map[data->cor.y][data->cor.x] = data->cor.c;
	}
	else if(keycode ==  KEY_DOWN || keycode == KEY_S)
	{
		data->map.map[data->cor.y][data->cor.x] = '2';
		data->cor.y += 1;
		data->map.map[data->cor.y][data->cor.x] = data->cor.c;
	}
	else if(keycode ==  KEY_LEFT || keycode == KEY_A)
	{
		data->map.map[data->cor.y][data->cor.x] = '2';
		data->cor.x -= 1;
		data->map.map[data->cor.y][data->cor.x] = data->cor.c;
	}
	else if(keycode ==  KEY_RIGHT || keycode == KEY_D)
	{
		data->map.map[data->cor.y][data->cor.x] = '2';
		data->cor.x += 1;
		data->map.map[data->cor.y][data->cor.x] = data->cor.c;
	}
}


int	ft_exit_handling(void *param)   // 이건 크로스 표시 컨트롤
{
	t_data	*data;

	data = (t_data *)param;
	mlx_destroy_image(data->mlx, data->img.img);
	mlx_destroy_window(data->mlx, data->win);
	mlx_destroy_display(data->mlx);
	free(data->mlx);
	exit (0);
}

void	init_cub3d_program(t_data *data)
{
	data->width = 2000;
	data->height = 2000;
	data->img.width = data->width;
	data->img.height = data->height;
	data->x_offset = 500;
	data->y_offset = 500;
}

void find_obj(t_data *data)
{
	data->cor.color = 0xFF0000;
	for (int y = 0; y < data->map.map_height; y++)
	{
		for (int x = 0; x < data->map.map_width; x++)
		{
			if ((ft_isinstr(data->map.map[y][x], "NSWE")))
			{
				data->cor.c = data->map.map[y][x];
				data->cor.x = x;
				data->cor.y = y;
				return ;
			}
		}
	}
}

int     main(int argc, char **argv)
{
    t_data  data;
    int fd;

   	map_parsing(argv[1], &data);
    data.mlx = mlx_init();
    init_cub3d_program(&data);    //데이타를 초기화 합니다
	find_obj(&data);
    mlx_get_screen_size(data.mlx, &data.width, &data.height);
    data.win = mlx_new_window (data.mlx, data.width, data.height, "LOVE");
    data.img.img = mlx_new_image(data.mlx, data.img.width, data.img.height);
    data.img.buffer = mlx_get_data_addr(data.img.img, &data.img.pixel_bits,
			&data.img.line_bytes, &data.img.endian);
    //여기에 파싱한 맵 데이터를 이미지로 찍어내는 함수를 넣어야할 듯 합니다
	draw_map_from_array(&data);
	// mlx_put_image_to_window(data.mlx, data.win, data.img.img, 0, 0);
    mlx_key_hook(data.win, ft_key_handling, &data);
    mlx_hook(data.win, 17, 0, ft_exit_handling, &data);
    // print_all(&(data.map));
    mlx_loop (data.mlx);
}
