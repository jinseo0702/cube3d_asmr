#include <sys/time.h>
#include "../../minilibx-linux/mlx.h"
#include "../../include/platform.h"

#define XK_ESCAPE 65307
#define XK_LEFT 65361
#define XK_UP 65362
#define XK_RIGHT 65363
#define XK_DOWN 65364

typedef struct s_mlx
{
	void	*mlx;
	void	*win;
	void	*img;
	char	*buffer;
	int		pixel_bits;
	int		line_bytes;
	int		endian;
	t_game	*game;
	double	prev;
	int		frames;
}	t_mlx;

static double	now_seconds(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec + tv.tv_usec / 1e6);
}

static int	set_key(t_game *game, int keysym, bool down)
{
	if (keysym == 'w' || keysym == XK_UP)
		game->keys.forward = down;
	else if (keysym == 's' || keysym == XK_DOWN)
		game->keys.back = down;
	else if (keysym == 'a')
		game->keys.strafe_l = down;
	else if (keysym == 'd')
		game->keys.strafe_r = down;
	else if (keysym == XK_LEFT)
		game->keys.turn_l = down;
	else if (keysym == XK_RIGHT)
		game->keys.turn_r = down;
	else if (keysym == XK_ESCAPE)
		game->keys.quit = true;
	else if (keysym == 'm' && down)
		game->show_minimap = !game->show_minimap;
	return (0);
}

static int	on_key_press(int keysym, void *param)
{
	t_mlx	*ctx;

	ctx = (t_mlx *)param;
	return (set_key(ctx->game, keysym, true));
}

static int	on_key_release(int keysym, void *param)
{
	t_mlx	*ctx;

	ctx = (t_mlx *)param;
	return (set_key(ctx->game, keysym, false));
}

static int	on_close(void *param)
{
	t_mlx	*ctx;

	ctx = (t_mlx *)param;
	ctx->game->keys.quit = true;
	return (0);
}

static void	present(t_mlx *ctx)
{
	t_fb	*fb;
	int		y;

	fb = &ctx->game->fb;
	y = 0;
	while (y < fb->height)
	{
		ft_memcpy(ctx->buffer + y * ctx->line_bytes,
			fb->pixels + y * fb->width, fb->width * sizeof(uint32_t));
		y++;
	}
	mlx_put_image_to_window(ctx->mlx, ctx->win, ctx->img, 0, 0);
}

static int	on_frame(void *param)
{
	t_mlx	*ctx;
	double	now;

	ctx = (t_mlx *)param;
	now = now_seconds();
	if (ctx->game->keys.quit || (ctx->game->max_frames > 0
			&& ctx->frames++ >= ctx->game->max_frames))
	{
		mlx_loop_end(ctx->mlx);
		return (0);
	}
	game_update(ctx->game, now - ctx->prev);
	ctx->prev = now;
	present(ctx);
	return (0);
}

int	platform_run(t_game *game)
{
	t_mlx	ctx;

	ft_memset(&ctx, 0, sizeof(t_mlx));
	ctx.game = game;
	ctx.mlx = mlx_init();
	if (!ctx.mlx)
		return (error_msg("mlx_init failed (is an X server running?)"));
	ctx.win = mlx_new_window(ctx.mlx, game->fb.width, game->fb.height,
			WIN_TITLE);
	ctx.img = mlx_new_image(ctx.mlx, game->fb.width, game->fb.height);
	if (!ctx.win || !ctx.img)
		return (error_msg("mlx window/image creation failed"));
	ctx.buffer = mlx_get_data_addr(ctx.img, &ctx.pixel_bits,
			&ctx.line_bytes, &ctx.endian);
	mlx_hook(ctx.win, 2, 1L << 0, on_key_press, &ctx);
	mlx_hook(ctx.win, 3, 1L << 1, on_key_release, &ctx);
	mlx_hook(ctx.win, 17, 0, on_close, &ctx);
	mlx_loop_hook(ctx.mlx, on_frame, &ctx);
	ctx.prev = now_seconds();
	mlx_loop(ctx.mlx);
	mlx_destroy_image(ctx.mlx, ctx.img);
	mlx_destroy_window(ctx.mlx, ctx.win);
	mlx_destroy_display(ctx.mlx);
	free(ctx.mlx);
	return (0);
}
