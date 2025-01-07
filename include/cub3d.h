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

int map_parsing(char *map);
void init_t_map(t_map *map_data);
int check_size(t_map *map_data);
int check_arg(char *str);

#endif