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
    int x;
    int y;
    int color;
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
    char *NO;
    char *SO;
    char *WE;
    char *EA;
    char *F;
    char *C;
    char **map;
}   t_map;

typedef struct  s_data 
{
    void    *mlx;       //mlx 함수를 쓰기위한 값
    void    *win;       //mlx win 함수를 쓰기위한 void 값
    int						width;      //화면 자체의 길이
	int						height;     //화면 자체의 높이
    int     x_offset;
    int     y_offset;
    t_map   map;
    t_allimg   img;            //이미지를 넣을 때 필요한 구조체
}               t_data;

#endif