# ifndef STRUCT_H
# define STRUCT_H

typedef struct s_allimg
{
	void					*img;   //이미지
    char					*buffer;    //mlx_get_data_addr 이 함수 쓸때 저장할거
	int						width;      //이미지의 길이
	int						height;     //이미지의 높이
    int						pixel_bits; //mlx_new_image 이 함수 쓸때, 사용할 픽셀 비트
	int						line_bytes; //mlx_new_image 이 함수 쓸 때, 사용한 라인의 바이트
    int						endian;     //mlx_new_image 이 함수 쓸 때, 사용할 엔디안 값
}			t_allimg;

typedef struct s_two_coordi_node
{
    double x;      // 소수점 위치를 위해 double로 변경
    double y;      // 소수점 위치를 위해 double로 변경
    int color;
    char c;
    double dir;    // 플레이어 방향 (라디안) 추가
}               t_two_coordi_node;

typedef struct s_draw_func
{
    int dx;
    int dy;
    int sx;
    int sy;
    int err;
    int e2;
}               t_draw_func;

typedef struct s_map
{
    int fd;
    int high;
    int map_width;
    int map_height;//이 아래에 status를 넣을지 말지 고민중 입니다.
    int exf;//exception_falg;
    char *NO;
    char *SO;
    char *WE;
    char *EA;
    int F;
    int C;
    char **map;
}   t_map;

typedef struct s_ray
{
    double dir_x;        // 광선의 X 방향 (코사인 값)
    double dir_y;        // 광선의 Y 방향 (사인 값)
    
    int map_x;           // 광선이 현재 지나가고 있는 맵의 X 좌표
    int map_y;           // 광선이 현재 지나가고 있는 맵의 Y 좌표
    
    int step_x;          // X 방향으로 이동할 때 +1 또는 -1
    int step_y;          // Y 방향으로 이동할 때 +1 또는 -1
    
    double side_dist_x;  // 현재 위치에서 다음 X 격자선까지의 거리
    double side_dist_y;  // 현재 위치에서 다음 Y 격자선까지의 거리
    
    double delta_dist_x; // X 격자선 사이의 거리
    double delta_dist_y; // Y 격자선 사이의 거리
    
    int hit;             // 벽에 부딪혔는지 여부 (0: 아니오, 1: 예)
    int side;            // 부딪힌 벽이 어느 면인지 (0: X면, 1: Y면)
    
    double perp_wall_dist; // 벽까지의 수직 거리 (화면의 깊이를 계산할 때 사용)
} t_ray;

typedef struct  s_data 
{
    void    *mlx;       //mlx 함수를 쓰기위한 값
    void    *win;       //mlx win 함수를 쓰기위한 void 값
    int						width;      //화면 자체의 길이
	int						height;     //화면 자체의 높이
    int     x_offset;
    int     y_offset;
    double ray_angle;
    double ray_len;
    double fov;
    int ray_count;
    int view_mode;      // 화면 모드 (0: 2D 맵, 1: 3D 뷰)
    t_map   map;
    t_allimg   img;            //이미지를 넣을 때 필요한 구조체
    t_two_coordi_node cor;//초기화 함수 없음
    t_ray ray;
    void    *north;
    void    *south;
    void    *west;
    void    *east;
}               t_data;

#endif