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
		if (data->map.map[data->cor.y - 1][data->cor.x] != '1' && data->map.map[data->cor.y - 1][data->cor.x] != 'X')
		{
			data->map.map[data->cor.y][data->cor.x] = '2';
			data->cor.y -= 1;
			data->map.map[data->cor.y][data->cor.x] = data->cor.c;
		}
		draw_map_from_array(data);
	}
	else if(keycode ==  KEY_DOWN || keycode == KEY_S)
	{
		if (data->map.map[data->cor.y + 1][data->cor.x] != '1' && data->map.map[data->cor.y + 1][data->cor.x] != 'X')
		{
			data->map.map[data->cor.y][data->cor.x] = '2';
			data->cor.y += 1;
			data->map.map[data->cor.y][data->cor.x] = data->cor.c;
		}

		draw_map_from_array(data);
	}
	else if(keycode ==  KEY_LEFT || keycode == KEY_A)
	{
		if (data->map.map[data->cor.y][data->cor.x - 1] != '1' && data->map.map[data->cor.y][data->cor.x - 1] != 'X')
		{
			data->map.map[data->cor.y][data->cor.x] = '2';
			data->cor.x -= 1;
			data->map.map[data->cor.y][data->cor.x] = data->cor.c;
		}
		
		draw_map_from_array(data);
	}
	else if(keycode ==  KEY_RIGHT || keycode == KEY_D)
	{
		if (data->map.map[data->cor.y][data->cor.x + 1] != '1' && data->map.map[data->cor.y][data->cor.x + 1] != 'X')
		{
			data->map.map[data->cor.y][data->cor.x] = '2';
			data->cor.x += 1;
			data->map.map[data->cor.y][data->cor.x] = data->cor.c;
		}
		
		draw_map_from_array(data);
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
	data->ray_len = 10.0;
	data->ray_count = 120;
	data->fov = M_PI / 3;
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


t_ray cast_single_ray(t_data *game, double angle)
{
    t_ray ray;
    
    // 광선 방향 벡터 계산
    ray.dir_x = cos(angle);
    ray.dir_y = sin(angle);
    
    // 플레이어가 있는 맵 좌표 계산
    ray.map_x = (int)game->cor.x;
    ray.map_y = (int)game->cor.y;
    
    // 광선이 다음 x, y 격자선으로 이동하는 데 필요한 거리 계산
    ray.delta_dist_x = (ray.dir_x == 0) ? 1e30 : fabs(1 / ray.dir_x);
    ray.delta_dist_y = (ray.dir_y == 0) ? 1e30 : fabs(1 / ray.dir_y);
    
    // 광선이 나아갈 방향과 첫 번째 격자선까지의 거리 계산
    if (ray.dir_x < 0)
    {
        ray.step_x = -1;
        ray.side_dist_x = (game->cor.x - ray.map_x) * ray.delta_dist_x;
    }
    else
    {
        ray.step_x = 1;
        ray.side_dist_x = (ray.map_x + 1.0 - game->cor.x) * ray.delta_dist_x;
    }
    
    if (ray.dir_y < 0)
    {
        ray.step_y = -1;
        ray.side_dist_y = (game->cor.y - ray.map_y) * ray.delta_dist_y;
    }
    else
    {
        ray.step_y = 1;
        ray.side_dist_y = (ray.map_y + 1.0 - game->cor.y) * ray.delta_dist_y;
    }
    
    // 벽을 찾을 때까지 광선을 칸 단위로 이동
    ray.hit = 0;
    while (ray.hit == 0)
    {
        // x, y 방향 중 더 가까운 격자선 선택
        if (ray.side_dist_x < ray.side_dist_y)
        {
            ray.side_dist_x += ray.delta_dist_x;
            ray.map_x += ray.step_x;
            ray.side = 0;
        }
        else
        {
            ray.side_dist_y += ray.delta_dist_y;
            ray.map_y += ray.step_y;
            ray.side = 1;
        }
        
        // 현재 위치에 벽이 있는지 확인
// 현재 위치에 벽이 있는지 확인
		if (ray.map_y < 0 || ray.map_y >= game->map.map_height || 
    		ray.map_x < 0 || ray.map_x >= game->map.map_width || 
    		game->map.map[ray.map_y][ray.map_x] == '1' || 
    		game->map.map[ray.map_y][ray.map_x] == 'X')
		{
    		ray.hit = 1;
		}
		
    }
    
    // 벽까지의 수직 거리 계산
	if (ray.side == 0)
    ray.perp_wall_dist = fabs((ray.map_x - game->cor.x + 
                         (1 - ray.step_x) / 2) / ray.dir_x);
	else
    ray.perp_wall_dist = fabs((ray.map_y - game->cor.y + 
                         (1 - ray.step_y) / 2) / ray.dir_y);
    
    // 여기서 필요에 따라 더 많은 정보를 계산할 수 있어
    // 예: 벽의 텍스처 좌표, 정확한 충돌 지점 등
    
    return ray;
}

// void draw_ray(t_data *data, double start_x, double start_y, double ray_angle)
// {
//     double ray_x = start_x;
//     double ray_y = start_y;
//     double delta_x = cos(ray_angle);
//     double delta_y = sin(ray_angle);
//     int map_x, map_y;
//     for (int i = 0; i < MAX_DEPTH; i++)  // 레이를 최대 탐색 거리까지 쏜다
//     {
//         ray_x += delta_x * 1;  // 1 픽셀씩 전진
//         ray_y += delta_y * 1;
//         map_x = (int)(ray_x / TILE_SIZE);
//         map_y = (int)(ray_y / TILE_SIZE);
//         // 벽(1)에 충돌하면 중단
//         if (data->map.map[map_y][map_x] == '1')
//             break;
//         // 레이의 진행 방향을 시각적으로 표현 (디버깅용)
//         mlx_pixel_put(data->mlx, data->win, (int)ray_x, (int)ray_y, 0xFF0000);
//     }
// }
// void draw_rays(t_data *data)
// {
// 	double player_dir = -M_PI / 2; // 북쪽 방향으로 설정
//     double player_x = data->cor.x * TILE_SIZE + TILE_SIZE / 2;
//     double player_y = data->cor.y * TILE_SIZE + TILE_SIZE / 2;
//     double angle = player_dir - (FOV / 2) * (M_PI / 180); // 시야각 시작점
//     for (int i = 0; i < NUM_RAYS; i++)
//     {
//         draw_ray(data, player_x, player_y, angle);
//         angle += (FOV / NUM_RAYS) * (M_PI / 180); // 각도 조정
//     }
// }

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
	load_textures(&data);	//여기에 그림 파일 텍스쳐를 로드 해둡니다. 2025.03.11
	draw_map_from_array(&data);
	// mlx_put_image_to_window(data.mlx, data.win, data.img.img, 0, 0);
    mlx_key_hook(data.win, ft_key_handling, &data);
    mlx_hook(data.win, 17, 0, ft_exit_handling, &data);
    // print_all(&(data.map));
    mlx_loop (data.mlx);
}

