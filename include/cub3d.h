#ifndef CUB3D_H
# define CUB3D_H

# include <stdint.h>
# include <stdbool.h>
# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include <fcntl.h>
# include <unistd.h>
# include "../libft/libft.h"

# define WIN_TITLE "cub3D"
# define WIN_WIDTH 1280
# define WIN_HEIGHT 720
# define FOV 0.66
# define MOVE_SPEED 3.0
# define ROT_SPEED 2.4
# define COLLISION_RADIUS 0.2

enum e_side
{
	NORTH = 0,
	SOUTH = 1,
	WEST = 2,
	EAST = 3
};

typedef struct s_tex
{
	int			width;
	int			height;
	uint32_t	*pixels;
}	t_tex;

typedef struct s_fb
{
	int			width;
	int			height;
	uint32_t	*pixels;
}	t_fb;

typedef struct s_map
{
	char		**grid;
	int			width;
	int			height;
	char		*tex_path[4];
	uint32_t	floor_color;
	uint32_t	ceil_color;
	double		spawn_x;
	double		spawn_y;
	char		spawn_dir;
}	t_map;

typedef struct s_player
{
	double	x;
	double	y;
	double	dir_x;
	double	dir_y;
	double	plane_x;
	double	plane_y;
}	t_player;

typedef struct s_keys
{
	bool	forward;
	bool	back;
	bool	strafe_l;
	bool	strafe_r;
	bool	turn_l;
	bool	turn_r;
	bool	quit;
}	t_keys;

typedef struct s_ray
{
	double	dir_x;
	double	dir_y;
	int		map_x;
	int		map_y;
	double	side_dist_x;
	double	side_dist_y;
	double	delta_x;
	double	delta_y;
	int		step_x;
	int		step_y;
	int		side;
	double	dist;
}	t_ray;

typedef struct s_game
{
	t_map		map;
	t_player	player;
	t_tex		tex[4];
	t_fb		fb;
	t_keys		keys;
	bool		show_minimap;
	int			max_frames;
}	t_game;

/* parser */
int			parse_scene(const char *path, t_game *game);
int			parse_config_line(char *line, t_map *map, int *found);
int			parse_rgb(const char *str, uint32_t *out);
int			build_grid(char **lines, int start, int count, t_map *map);
int			validate_map(t_map *map);
int			error_msg(const char *msg);
char		**read_all_lines(const char *path, int *count);
void		free_lines(char **lines);

/* engine */
int			game_init(t_game *game, const char *map_path);
void		game_update(t_game *game, double dt);
void		move_player(t_game *game, double dt);
bool		is_walkable(t_map *map, double x, double y);
void		game_destroy(t_game *game);

/* render */
void		render_frame(t_game *game);
void		render_minimap(t_game *game);
int			fb_init(t_fb *fb, int width, int height);
void		fb_clear(t_fb *fb, uint32_t color);
void		fb_put_pixel(t_fb *fb, int x, int y, uint32_t color);
int			fb_write_ppm(t_fb *fb, const char *path);
int			tex_load_xpm(t_tex *tex, const char *path);

#endif
