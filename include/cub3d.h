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

/* 키 정의 */
# define KEY_UP     65362
# define KEY_DOWN   65364
# define KEY_LEFT   65361
# define KEY_RIGHT  65363
# define KEY_W      119
# define KEY_S      115
# define KEY_A      97
# define KEY_D      100
# define KEY_ESCAPE 65307

/* 게임 관련 상수 */
# define TRUE  1
# define FALSE  0


/* 메인 및 초기화 함수 */
int     check_args(int argc, char **argv, t_data *data);
void    init_mlx(t_data *data);
void    init_window(t_data *data);
void    init_image(t_data *data);
void    init_cub3d_program(t_data *data);

/* 맵 파싱 함수 */
int     map_check(char *map, t_data *data);
void    init_t_map(t_map *map_data);
int     check_size(t_map *map_data);
int     check_arg(char *str);
int     dup_info(char *str, t_map *map_data);
void     is_right_map(char *str, t_map *map_data);
int     insert_data(t_map *map_data, char *map);

/* 맵 검증 함수 */
int     is_valid_map_char(char c);
void    copy_and_process_map_line(t_map *map_data, char *str);
void     validate_map_line(char *str);

/* 컬러 파싱 함수 */
void     validate_rgb(int r, int g, int b);
char    **split_rgb_parts(char *color_str);
char    **split_color_line(char *line);
int     create_color(int r, int g, int b);
int     parse_color(char *line);

/* 이벤트 관련 함수 */
void    setup_events(t_data *data);
int     ft_key_handling(int keycode, t_data *data);
int     ft_exit_handling(void *param);
void    update_view(t_data *data);
int     handle_movement_keys(int keycode, t_data *data);

/* 플레이어 움직임 함수 */
void move_player(t_data *data, int flag);
void    rotate_player(t_data *data, int direction);

/* 플러드 필 알고리즘 */
void    solve_dfs(char **map, int x, int y);
int     flood_fill(char **map);

/* 유틸리티 함수 */
void    print_all(t_map *map);
int     ft_arraylen(char **array);
void    ft_free_array(char **array);
void    find_obj(t_data *data);
void    solve_Dfs2(char **map, int x, int y);

/* 광선 캐스팅 관련 함수 */
t_ray   cast_single_ray(t_data *game, double angle);
void    init_ray_direction(t_ray *ray, double angle);
void    init_ray_map_pos(t_ray *ray, t_data *game);
void    calculate_delta_dist(t_ray *ray);
void    calculate_step_side_dist(t_ray *ray, t_data *game); 
void    calculate_step_side_dist_x(t_ray *ray, t_data *game);
void    calculate_step_side_dist_y(t_ray *ray, t_data *game);
int     check_map_bounds(t_ray *ray, t_data *game);
int     check_wall_hit(t_ray *ray, t_data *game);
void    perform_dda(t_ray *ray, t_data *game);
void    calculate_wall_distance(t_ray *ray, t_data *game);
int     check_wall_collision(t_ray *ray, char **map, int width, int height);
void    step_to_next_grid(t_ray *ray);


/* 3D 렌더링 관련 함수 */
void    draw_floor_ceiling(t_data *data);
void    draw_walls_3d(t_data *data);
double correct_ray_distance(double corrected_dist, double ray_angle, t_data *data);
int     calculate_wall_height(t_data *data, double corrected_dist);
void    render_3d(t_data *data);
void    init_player_direction(t_data *data);
void process_wall_slice(t_data *data, int x, t_ray *ray, double ray_angle);


/* 텍스처 관련 함수 */
void    init_textures(t_data *data);
int     get_tex_x(t_data *data, t_ray *ray);
void    draw_wall_line(t_data *data, int x, int start, int end, t_wall_tex tex);
void    draw_textured_wall(t_data *data, int x, t_ray ray, int wall_height);
void    calculate_wall_bounds(int wall_height, t_data *data, int *draw_start, int *draw_end);
t_allimg *select_texture_x_side(t_ray ray, t_data *data);
t_allimg *select_texture_y_side(t_ray ray, t_data *data);
t_allimg *select_texture(t_ray ray, t_data *data);
int get_texture_color(t_wall_tex *tex, double tex_pos); 


#endif
