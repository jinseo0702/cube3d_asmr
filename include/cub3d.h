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
# define TILE_SIZE  32
# define FOV        60
# define NUM_RAYS   120
# define MAX_DEPTH  20

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
int     insert_data(t_map *map_data, char *map);//수정 요망 

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
int     is_move_valid(t_data *data, double new_x, double new_y);
void    update_player_position(t_data *data, double new_x, double new_y);
void    process_wasd_movement(int keycode, t_data *data);
void    process_arrow_rotation(int keycode, t_data *data);

/* 플러드 필 알고리즘 */
void    solve_Dfs(char **map, int x, int y);
int     flood_fill(char **map);

/* 유틸리티 함수 */
void    print_all(t_map *map);
int     ft_arraylen(char **array);
void    ft_free_array(char **array);
void    init_p(t_two_coordi_node p, int x, int y, int color);
void    find_obj(t_data *data);
void solve_Dfs2(char **map, int x, int y);

/* 라인 그리기 함수 */
void    draw_line_to_image(t_data *data, int x1, int y1, int x2, int y2, int color);
void    init_line_draw(int *dx, int *dy, int *sx, int *sy, int x1, int y1, int x2, int y2);
void    set_line_pixel(t_data *data, int x, int y, int color);
void    process_line_algorithm(t_data *data, int x1, int y1, int x2, int y2, int color);

/* 광선 캐스팅 관련 함수 */
t_ray   cast_single_ray(t_data *game, double angle);
void    init_ray_direction(t_ray *ray, double angle);
void    init_ray_map_pos(t_ray *ray, t_data *game);
void    calculate_delta_dist(t_ray *ray);
void    calculate_step_side_dist_x(t_ray *ray, t_data *game);
void    calculate_step_side_dist_y(t_ray *ray, t_data *game);
int     check_map_bounds(t_ray *ray, t_data *game);
int     check_wall_hit(t_ray *ray, t_data *game);
void    perform_dda(t_ray *ray, t_data *game);
void    calculate_wall_distance(t_ray *ray, t_data *game);

/* 레이 시각화 함수 */
void    draw_rays(t_data *data);
double  limit_ray_length(double ray_length, t_data *data);
void    calculate_ray_endpoint(int start_x, int start_y, int *end_x, int *end_y,
        double ray_angle, double ray_length);
void    draw_single_ray(t_data *data, double ray_angle);
void    draw_map_from_array(t_data *data);

/* 3D 렌더링 관련 함수 */
void draw_floor_ceiling(t_data *data);
void    draw_walls_3d(t_data *data);
void    correct_ray_distance(double *corrected_dist, double ray_angle, t_data *data);
int     calculate_wall_height(t_data *data, double corrected_dist);
void    render_3d(t_data *data);
void    init_player_direction(t_data *data);
double  get_wall_x(t_data *data, t_ray *ray);

/* 텍스처 관련 함수 */
void    init_textures(t_data *data);
int     get_tex_x(t_data *data, t_ray *ray);
void    draw_wall_line(t_data *data, int x, int start, int end, t_wall_tex tex);
void    draw_textured_wall(t_data *data, int x, t_ray ray, int wall_height);
void    calculate_wall_bounds(int wall_height, t_data *data, int *draw_start, int *draw_end);
t_allimg *select_texture_x_side(t_ray ray, t_data *data);
t_allimg *select_texture_y_side(t_ray ray, t_data *data);
t_allimg *select_texture(t_ray ray, t_data *data);
void    setup_texture_info(t_wall_tex *tex, t_allimg *tex_img, t_data *data, t_ray *ray, 
        int wall_height);

#endif