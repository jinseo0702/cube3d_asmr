#include "cub3d.h"

// 생각해보아야할 것
//기본적으로 argv에 .cub파일만 받아오는지

int	ft_key_handling(int keycode, t_data *data)
{
	if (keycode == 65307)
	{
		mlx_destroy_image(data->mlx, data->img.img);
		mlx_destroy_window(data->mlx, data->win);
		mlx_destroy_display(data->mlx);
		free(data->mlx);
		exit(0);
	}
}
int	ft_exit_handling(void *param)
{
	t_data	*data;

	data = (t_data *)param;
	mlx_destroy_image(data->mlx, data->img.img);
	mlx_destroy_window(data->mlx, data->win);
	mlx_destroy_display(data->mlx);
	free(data->mlx);
	exit (0);
}

void	init_fdf_program(t_data *data)
{
	data->width = 2000;
	data->height = 2000;
	data->img.width = data->width;
	data->img.height = data->height;
}

int     main(int argc, char **argv)
{
    t_data  data;
    int fd;

    /*
    fd = open (argv[1], O_RDONLY);
    if (fd < 0)
	{	
		perror("ERROR");
		exit(1);
	}
    close(fd);
    */
    data.mlx = mlx_init();
    init_fdf_program(&data);
    mlx_get_screen_size(data.mlx, &data.width, &data.height);
    data.win = mlx_new_window (data.mlx, data.width, data.height, "LOVE");
    data.img.img = mlx_new_image(data.mlx, data.img.width, data.img.height);
    mlx_key_hook(data.win, ft_key_handling, &data);
    mlx_hook(data.win, 17, 0, ft_exit_handling, &data);
    mlx_loop (data.mlx);
}
