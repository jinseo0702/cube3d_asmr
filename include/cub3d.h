#ifndef CUB3D_H
# define CUB3D_H

# include "./struct.h"
# include "../minilibx-linux/mlx.h"
# include "../minilibx-linux/mlx_int.h"
# include "../libft/libft.h"
# include "../libft/gnl_check_bonus/get_next_line.h"
# include "../libft/gnl_check_bonus/get_next_line_bonus.h"
# include <fcntl.h>
# include <math.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <stdio.h>//나중에 지워야함

//---------------------------------------------
#	define KEY_UP	65362
#	define KEY_DOWN	65364
#	define KEY_LEFT	65361
#	define KEY_RIGHT	65363
#	define KEY_W	119
#	define KEY_S	115
#	define KEY_A	97
#	define KEY_D	100
#	define KEY_M	109             // 뷰 모드 전환 키 (M)
#	define KEY_ESCAPE	65307

#define TILE_SIZE 32
#define FOV 60 // 플레이어의 시야각
#define NUM_RAYS 120 // 시야각 내에서 몇 개의 레이를 쏠지 (화질에 영향)
#define MAX_DEPTH 20 // 레이의 최대 탐색 거리
//---------------------------------------------



int map_parsing(char *map, t_data *data);
void init_t_map(t_map *map_data);
int check_size(t_map *map_data);
int check_arg(char *str);
int dup_info(char *str, t_map *map_data);
int is_right_map(char *str, t_map *map_data);
int insert_data(t_map *map_data, char *map);


//---------------------------------------------
void print_all(t_map *map);
int ft_arraylen(char **array);
void ft_free_array(char **array);
void load_textures(t_data *data);
//---------------------------------------------

//---------------------------------------------

int	ft_key_handling(int keycode, t_data *data);  //이건 esc 컨트롤
int	ft_exit_handling(void *param);   // 이건 크로스 표시 컨트롤
void	init_cub3d_program(t_data *data);
void put_pixel_to_image(t_allimg *img, int x, int y, int color);
void init_draw_info(t_draw_func *draw, t_two_coordi_node p1, t_two_coordi_node p2);
void draw_map_from_array(t_data *data);
void    init_p(t_two_coordi_node p, int x, int y, int color);
void find_obj(t_data *data);
//---------------------------------------------


//---------------------------------------------
//flood fill algorithm
void solve_Dfs(char **map, int x, int y, int *status);
int flud_fill(char **map);

t_ray cast_single_ray(t_data *game, double angle);
void set_ray(t_data *data);
void draw_line_to_image(t_data *data, int x1, int y1, int x2, int y2, int color);
void draw_rays(t_data *data);
void draw_ray(t_data *data, double start_x, double start_y, double ray_angle);

// 3D 렌더링 관련 함수들
void draw_floor_ceiling(t_data *data);
void draw_walls_3d(t_data *data);
void render_3d(t_data *data);
void init_player_direction(t_data *data);
double get_wall_x(t_data *data, t_ray *ray);

#endif