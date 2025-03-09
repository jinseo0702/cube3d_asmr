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
#	define KEY_ESCAPE	65307
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
void draw_line_to_image(t_data *data, int x1, int y1, int x2, int y2, int color);
void draw_rays(t_data *data);

#endif

