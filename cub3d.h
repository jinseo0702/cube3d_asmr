#ifndef CUB3D_H
# define CUB3D_H

# include "./minilibx-linux/mlx.h"
# include <fcntl.h>
# include <math.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>

typedef struct s_img
{
	void					*img;
	int						width;
	int						height;
    int						endian;
}			t_img;

typedef struct  s_data 
{
    void    *mlx;
    void    *win;
    int						width;
	int						height;
    t_img   img;
}               t_data;



#endif