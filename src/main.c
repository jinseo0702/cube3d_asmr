#include "../include/cub3d.h"

// 생각해보아야할 것
//기본적으로 argv에 .cub파일만 받아오는지


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
        
        // 맵 범위 체크 추가 - 중요한 안전장치!
        if (ray.map_y < 0 || ray.map_y >= game->map.map_height || 
            ray.map_x < 0 || ray.map_x >= game->map.map_width)
        {
            ray.hit = 1;
            ray.perp_wall_dist = 20.0; // 맵 밖으로 나간 경우 큰 값 설정
            break;
        }
        // 현재 위치에 벽이 있는지 확인
        else if (game->map.map[ray.map_y][ray.map_x] == '1' || 
                 game->map.map[ray.map_y][ray.map_x] == 'X')
        {
            ray.hit = 1;
        }
    }
    
    // 벽까지의 수직 거리 계산
    if (ray.side == 0)
        ray.perp_wall_dist = fabs((ray.map_x - game->cor.x + (1 - ray.step_x) / 2) / ray.dir_x);
    else
        ray.perp_wall_dist = fabs((ray.map_y - game->cor.y + (1 - ray.step_y) / 2) / ray.dir_y);
    
    return ray;
}

// 방향 초기화 함수 추가
void init_player_direction(t_data *data)
{
    // 플레이어의 초기 방향 설정 (N,S,E,W)
    if (data->cor.c == 'N')
        data->cor.dir = -M_PI / 2;  // 북쪽은 -90도
    else if (data->cor.c == 'S')
        data->cor.dir = M_PI / 2;   // 남쪽은 90도
    else if (data->cor.c == 'E')
        data->cor.dir = 0;          // 동쪽은 0도
    else if (data->cor.c == 'W')
        data->cor.dir = M_PI;       // 서쪽은 180도
}

// 키 이벤트 처리 함수 수정: 3D 이동과 회전 추가
int ft_key_handling(int keycode, t_data *data)
{
    // 화면 모드 전환 (M 키)
    if (keycode == KEY_M)
    {
        data->view_mode = !data->view_mode;
        if (data->view_mode)
            render_3d(data);
        else
            draw_map_from_array(data);
    }
    // ESC 키 처리 (종료)
    else if (keycode == KEY_ESCAPE)
    {
        mlx_destroy_image(data->mlx, data->img.img);
        mlx_destroy_window(data->mlx, data->win);
        mlx_destroy_display(data->mlx);
        free(data->mlx);
        exit(0);
    }
    // 전진 이동 (W 키 또는 위쪽 화살표)
    else if(keycode == KEY_UP || keycode == KEY_W)
    {
        double new_x = data->cor.x + cos(data->cor.dir) * 0.1;
        double new_y = data->cor.y + sin(data->cor.dir) * 0.1;
        
        if (data->map.map[(int)new_y][(int)new_x] != '1' && 
            data->map.map[(int)new_y][(int)new_x] != 'X')
        {
            data->map.map[(int)data->cor.y][(int)data->cor.x] = '2';
            data->cor.x = new_x;
            data->cor.y = new_y;
            data->map.map[(int)data->cor.y][(int)data->cor.x] = data->cor.c;
        }
    }
    // 후진 이동 (S 키 또는 아래쪽 화살표)
    else if(keycode == KEY_DOWN || keycode == KEY_S)
    {
        double new_x = data->cor.x - cos(data->cor.dir) * 0.1;
        double new_y = data->cor.y - sin(data->cor.dir) * 0.1;
        
        if (data->map.map[(int)new_y][(int)new_x] != '1' && 
            data->map.map[(int)new_y][(int)new_x] != 'X')
        {
            data->map.map[(int)data->cor.y][(int)data->cor.x] = '2';
            data->cor.x = new_x;
            data->cor.y = new_y;
            data->map.map[(int)data->cor.y][(int)data->cor.x] = data->cor.c;
        }
    }
    // 왼쪽으로 이동 (A 키)
    else if(keycode == KEY_A)
    {
        double new_x = data->cor.x - cos(data->cor.dir + M_PI / 2) * 0.1;
        double new_y = data->cor.y - sin(data->cor.dir + M_PI / 2) * 0.1;
        
        if (data->map.map[(int)new_y][(int)new_x] != '1' && 
            data->map.map[(int)new_y][(int)new_x] != 'X')
        {
            data->map.map[(int)data->cor.y][(int)data->cor.x] = '2';
            data->cor.x = new_x;
            data->cor.y = new_y;
            data->map.map[(int)data->cor.y][(int)data->cor.x] = data->cor.c;
        }
    }
    // 오른쪽으로 이동 (D 키)
    else if(keycode == KEY_D)
    {
        double new_x = data->cor.x + cos(data->cor.dir + M_PI / 2) * 0.1;
        double new_y = data->cor.y + sin(data->cor.dir + M_PI / 2) * 0.1;
        
        if (data->map.map[(int)new_y][(int)new_x] != '1' && 
            data->map.map[(int)new_y][(int)new_x] != 'X')
        {
            data->map.map[(int)data->cor.y][(int)data->cor.x] = '2';
            data->cor.x = new_x;
            data->cor.y = new_y;
            data->map.map[(int)data->cor.y][(int)data->cor.x] = data->cor.c;
        }
    }
    // 왼쪽으로 회전 (왼쪽 화살표)
    else if(keycode == KEY_LEFT)
    {
        data->cor.dir -= 0.1;
        if (data->cor.dir < 0)
            data->cor.dir += 2 * M_PI;
    }
    // 오른쪽으로 회전 (오른쪽 화살표)
    else if(keycode == KEY_RIGHT)
    {
        data->cor.dir += 0.1;
        if (data->cor.dir > 2 * M_PI)
            data->cor.dir -= 2 * M_PI;
    }

    // 키 입력 후 화면 업데이트
    if (data->view_mode)
        render_3d(data);
    else
        draw_map_from_array(data);

    return (0);
}

// main.c 메인 함수 수정
// main.c 함수 수정

int main(int argc, char **argv)
{
    t_data data;
    int fd;

    if (argc != 2)
    {
        printf("Error\n잘못된 인자 개수입니다. ./cub3D [맵파일.cub] 형식으로 실행하세요.\n");
        return (1);
    }

    // 맵 파싱
    if (!map_parsing(argv[1], &data))
    {
        printf("Error\n맵 파싱에 실패했습니다.\n");
        return (1);
    }

    // MLX 초기화
    data.mlx = mlx_init();
    if (!data.mlx)
    {
        printf("Error\nMLX 초기화에 실패했습니다.\n");
        return (1);
    }

    // 프로그램 데이터 초기화
    init_cub3d_program(&data);
    
    // 플레이어 위치 찾기
    find_obj(&data);
    
    // 플레이어 초기 방향 설정
    init_player_direction(&data);
    
    // 화면 크기 가져오기 (여기서는 고정값을 사용)
    // mlx_get_screen_size는 창을 만들기 전에 호출할 수 없음
    data.width = 800;  // 적절한 해상도로 수정
    data.height = 600;
    
    // 창 생성
    data.win = mlx_new_window(data.mlx, data.width, data.height, "Cub3D");
    if (!data.win)
    {
        printf("Error\n윈도우 생성에 실패했습니다.\n");
        return (1);
    }
    
    // 이미지 생성
    data.img.img = mlx_new_image(data.mlx, data.width, data.height);
    data.img.buffer = mlx_get_data_addr(data.img.img, &data.img.pixel_bits,
            &data.img.line_bytes, &data.img.endian);
    
    // 텍스처 로드
    load_textures(&data);
    
    // 기본 모드는 3D 맵 (수정 가능)
    data.view_mode = 1;  // 1은 3D 모드, 0은 2D 모드
    
    // 이제 모든 초기화가 완료된 후에 3D 또는 2D 맵 표시
    if (data.view_mode)
        render_3d(&data);
    else
        draw_map_from_array(&data);
    
    // 키보드 및 창 종료 이벤트 설정
    mlx_key_hook(data.win, ft_key_handling, &data);
    mlx_hook(data.win, 17, 0, ft_exit_handling, &data);
    
    // MLX 루프 시작
    mlx_loop(data.mlx);
    
    return (0);
}