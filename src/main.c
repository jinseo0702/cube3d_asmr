#include "../include/cub3d.h"
#include "../include/platform.h"

static int	usage(void)
{
	ft_putstr_fd("Error\nusage: ./cub3D <map.cub> "
		"[--screenshot <out.ppm>] [--frames <n>]\n", 2);
	return (1);
}

static int	parse_args(int argc, char **argv, const char **shot, int *frames)
{
	int	i;

	i = 1;
	while (++i < argc)
	{
		if (ft_strncmp(argv[i], "--screenshot", 13) == 0 && i + 1 < argc)
			*shot = argv[++i];
		else if (ft_strncmp(argv[i], "--frames", 9) == 0 && i + 1 < argc)
			*frames = ft_atoi(argv[++i]);
		else
			return (-1);
	}
	return (0);
}

static int	run_headless(t_game *game, const char *out_path)
{
	int	ret;

	render_frame(game);
	ret = fb_write_ppm(&game->fb, out_path);
	game_destroy(game);
	return (ret < 0);
}

int	main(int argc, char **argv)
{
	t_game		game;
	const char	*screenshot;
	int			max_frames;

	screenshot = NULL;
	max_frames = 0;
	if (argc < 2 || parse_args(argc, argv, &screenshot, &max_frames) < 0)
		return (usage());
	if (game_init(&game, argv[1]) < 0)
		return (game_destroy(&game), 1);
	game.max_frames = max_frames;
	if (screenshot)
		return (run_headless(&game, screenshot));
	if (platform_run(&game) < 0)
		return (game_destroy(&game), 1);
	game_destroy(&game);
	return (0);
}
