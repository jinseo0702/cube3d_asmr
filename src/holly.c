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
// 벽 렌더링 전에 광선 보정을 처리하는 함수
void correct_ray_distance(double *corrected_dist, double ray_angle, t_data *data)
{
    double correction;
    
    correction = cos(ray_angle - data->cor.dir);
    *corrected_dist = *corrected_dist * correction;
    
    if (*corrected_dist < 0.1)
        *corrected_dist = 0.1;
}

// 벽 높이를 계산하는 함수
int calculate_wall_height(t_data *data, double corrected_dist)
{
    int wall_height;
    
    wall_height = (int)(data->height / corrected_dist * 0.5);
    return (wall_height);
}

// 3D 벽을 그리는 함수
void draw_walls_3d(t_data *data)
{
    double ray_angle;
    int x;
    t_ray ray;
    int wall_height;
    double corrected_dist;
    
    ray_angle = data->cor.dir - (data->fov / 2);
    x = 0;
    
    while (x < data->width)
    {
        ray = cast_single_ray(data, ray_angle);
        
        corrected_dist = ray.perp_wall_dist;
        correct_ray_distance(&corrected_dist, ray_angle, data);
        
        wall_height = calculate_wall_height(data, corrected_dist);
        
        draw_textured_wall(data, x, ray, wall_height);
        
        ray_angle += data->fov / data->width;
        x++;
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
void render_3d(t_data *data)
{
    // 천장과 바닥 먼저 그리기
    draw_floor_ceiling(data);
    
    // 그 다음 벽 그리기
    draw_walls_3d(data);
    
    // 최종 이미지를 윈도우에 표시
    mlx_put_image_to_window(data->mlx, data->win, data->img.img, 0, 0);
}

void	init_textures(t_data *data)
{
	int	width;
	int	height;

	// 텍스처 기본 크기 설정
	data->tex_n.width = TEX_WIDTH;
	data->tex_n.height = TEX_HEIGHT;
	data->tex_s.width = TEX_WIDTH;
	data->tex_s.height = TEX_HEIGHT;
	data->tex_w.width = TEX_WIDTH;
	data->tex_w.height = TEX_HEIGHT;
	data->tex_e.width = TEX_WIDTH;
	data->tex_e.height = TEX_HEIGHT;
	
	// 북쪽 텍스처 로드 및 검사
	printf("북쪽 텍스처 로드 시도:%s--\n", data->map.NO);
	data->north = mlx_xpm_file_to_image(data->mlx, data->map.NO, &width, &height);
	if (!data->north)
		printf("북쪽 텍스처 로드 실패!\n");
	else
		printf("북쪽 텍스처 로드 성공: %dx%d\n", width, height);
		
	// 남쪽 텍스처 로드 및 검사
	printf("남쪽 텍스처 로드 시도:%s--\n", data->map.SO);
	data->south = mlx_xpm_file_to_image(data->mlx, data->map.SO, &width, &height);
	if (!data->south)
		printf("남쪽 텍스처 로드 실패!\n");
	else
		printf("남쪽 텍스처 로드 성공: %dx%d\n", width, height);
		
	// 서쪽 텍스처 로드 및 검사
	printf("서쪽 텍스처 로드 시도:%s--\n", data->map.WE);
	data->west = mlx_xpm_file_to_image(data->mlx, data->map.WE, &width, &height);
	if (!data->west)
		printf("서쪽 텍스처 로드 실패!\n");
	else
		printf("서쪽 텍스처 로드 성공: %dx%d\n", width, height);
		
	// 동쪽 텍스처 로드 및 검사
	printf("동쪽 텍스처 로드 시도:%s--\n", data->map.EA);
	data->east = mlx_xpm_file_to_image(data->mlx, data->map.EA, &width, &height);
	if (!data->east)
		printf("동쪽 텍스처 로드 실패!\n");
	else
		printf("동쪽 텍스처 로드 성공: %dx%d\n", width, height);
	
	// 텍스처 구조체에 이미지 할당
	data->tex_n.img = data->north;
	data->tex_s.img = data->south;
	data->tex_w.img = data->west;
	data->tex_e.img = data->east;
	
	// 텍스처 픽셀 데이터 가져오기 (널 체크 추가)
	if (data->tex_n.img)
		data->tex_n.buffer = mlx_get_data_addr(data->tex_n.img, &data->tex_n.pixel_bits,
			&data->tex_n.line_bytes, &data->tex_n.endian);
	if (data->tex_s.img)
		data->tex_s.buffer = mlx_get_data_addr(data->tex_s.img, &data->tex_s.pixel_bits,
			&data->tex_s.line_bytes, &data->tex_s.endian);
	if (data->tex_w.img)
		data->tex_w.buffer = mlx_get_data_addr(data->tex_w.img, &data->tex_w.pixel_bits,
			&data->tex_w.line_bytes, &data->tex_w.endian);
	if (data->tex_e.img)
		data->tex_e.buffer = mlx_get_data_addr(data->tex_e.img, &data->tex_e.pixel_bits,
			&data->tex_e.line_bytes, &data->tex_e.endian);
}

// 벽 텍스처의 X 좌표 계산
int	get_tex_x(t_data *data, t_ray *ray)
{
	double	wall_x;
	int		tex_x;
	
	// 광선이 벽과 충돌한 정확한 위치 계산
	if (ray->side == 0)
		wall_x = data->cor.y + ray->perp_wall_dist * ray->dir_y;
	else
		wall_x = data->cor.x + ray->perp_wall_dist * ray->dir_x;
	wall_x -= floor(wall_x); // 소수점 부분만 사용
	
	// 텍스처의 x 좌표 계산
	tex_x = (int)(wall_x * (double)TEX_WIDTH);
	
	// 텍스처 좌표 반전 (방향에 따라)
	if ((ray->side == 0 && ray->dir_x > 0) || 
		(ray->side == 1 && ray->dir_y < 0))
		tex_x = TEX_WIDTH - tex_x - 1;
	
	return (tex_x);
}

// 벽의 한 줄을 그리는 함수
void	draw_wall_line(t_data *data, int x, int start, int end, t_wall_tex tex)
{
	int			y;
	int			tex_y;
	double		step;
	double		tex_pos;
	char		*dst;
	char		*src;
	int			color;
	
	// 텍스처 y 좌표 계산을 위한 준비
	step = (double)TEX_HEIGHT / tex.height;
	tex_pos = (start - data->height / 2 + tex.height / 2) * step;
	
	// 벽의 각 픽셀을 그림
	y = start;
	while (y < end)
	{
		tex_y = (int)tex_pos & (TEX_HEIGHT - 1);
		tex_pos += step;
		
		// 텍스처에서 해당 픽셀 색상 가져오기
		src = tex.buffer + (tex_y * tex.line_bytes + tex.tex_x * (tex.pixel_bits / 8));
		color = *(unsigned int *)src;
		
		// 이미지 버퍼에 색상 적용
		if (y >= 0 && y < data->height && x >= 0 && x < data->width)
		{
			dst = data->img.buffer + (y * data->img.line_bytes + x * (data->img.pixel_bits / 8));
			*(unsigned int *)dst = color;
		}
		y++;
	}
}

// 벽을 텍스처로 그리는 함수
void	draw_textured_wall(t_data *data, int x, t_ray ray, int wall_height)
{
	t_wall_tex	tex;
	int			draw_start;
	int			draw_end;
	t_allimg	*tex_img;
	
	// 그릴 벽의 시작점과 끝점 계산
	draw_start = -wall_height / 2 + data->height / 2;
	if (draw_start < 0)
		draw_start = 0;
	draw_end = wall_height / 2 + data->height / 2;
	if (draw_end >= data->height)
		draw_end = data->height - 1;
	
	// 벽 방향에 따라 텍스처 선택
	if (ray.side == 0)
	{
		if (ray.dir_x > 0)
			tex_img = &data->tex_e;
		else
			tex_img = &data->tex_w;
	}
	else
	{
		if (ray.dir_y > 0)
			tex_img = &data->tex_s;
		else
			tex_img = &data->tex_n;
	}
	
	// 텍스처 정보 설정
	tex.buffer = tex_img->buffer;
	tex.line_bytes = tex_img->line_bytes;
	tex.pixel_bits = tex_img->pixel_bits;
	tex.tex_x = get_tex_x(data, &ray);
	tex.height = wall_height;
	
	// 벽 그리기
	draw_wall_line(data, x, draw_start, draw_end, tex);
}