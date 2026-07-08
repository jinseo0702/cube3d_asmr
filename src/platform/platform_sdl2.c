#include <SDL.h>
#include "../../include/platform.h"

typedef struct s_sdl
{
	SDL_Window		*window;
	SDL_Renderer	*renderer;
	SDL_Texture		*texture;
}	t_sdl;

static void	sdl_destroy(t_sdl *sdl)
{
	if (sdl->texture)
		SDL_DestroyTexture(sdl->texture);
	if (sdl->renderer)
		SDL_DestroyRenderer(sdl->renderer);
	if (sdl->window)
		SDL_DestroyWindow(sdl->window);
	SDL_Quit();
}

static int	sdl_create(t_sdl *sdl, t_game *game)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		return (error_msg(SDL_GetError()));
	sdl->window = SDL_CreateWindow(WIN_TITLE, SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, game->fb.width, game->fb.height, 0);
	if (!sdl->window)
		return (error_msg(SDL_GetError()));
	sdl->renderer = SDL_CreateRenderer(sdl->window, -1,
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!sdl->renderer)
		sdl->renderer = SDL_CreateRenderer(sdl->window, -1,
				SDL_RENDERER_SOFTWARE);
	if (!sdl->renderer)
		return (error_msg(SDL_GetError()));
	sdl->texture = SDL_CreateTexture(sdl->renderer, SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_STREAMING, game->fb.width, game->fb.height);
	if (!sdl->texture)
		return (error_msg(SDL_GetError()));
	return (0);
}

static void	handle_key(t_game *game, SDL_Keycode key, bool down)
{
	if (key == SDLK_w || key == SDLK_UP)
		game->keys.forward = down;
	else if (key == SDLK_s || key == SDLK_DOWN)
		game->keys.back = down;
	else if (key == SDLK_a)
		game->keys.strafe_l = down;
	else if (key == SDLK_d)
		game->keys.strafe_r = down;
	else if (key == SDLK_LEFT)
		game->keys.turn_l = down;
	else if (key == SDLK_RIGHT)
		game->keys.turn_r = down;
	else if (key == SDLK_ESCAPE)
		game->keys.quit = true;
	else if (key == SDLK_m && down)
		game->show_minimap = !game->show_minimap;
}

static void	poll_events(t_game *game)
{
	SDL_Event	event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
			game->keys.quit = true;
		else if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
			handle_key(game, event.key.keysym.sym, true);
		else if (event.type == SDL_KEYUP)
			handle_key(game, event.key.keysym.sym, false);
	}
}

int	platform_run(t_game *game)
{
	t_sdl		sdl;
	uint64_t	now;
	uint64_t	prev;
	int			frames;

	ft_memset(&sdl, 0, sizeof(t_sdl));
	if (sdl_create(&sdl, game) < 0)
		return (sdl_destroy(&sdl), -1);
	frames = 0;
	prev = SDL_GetPerformanceCounter();
	while (!game->keys.quit)
	{
		poll_events(game);
		now = SDL_GetPerformanceCounter();
		game_update(game, (double)(now - prev)
			/ SDL_GetPerformanceFrequency());
		prev = now;
		SDL_UpdateTexture(sdl.texture, NULL, game->fb.pixels,
			game->fb.width * sizeof(uint32_t));
		SDL_RenderCopy(sdl.renderer, sdl.texture, NULL, NULL);
		SDL_RenderPresent(sdl.renderer);
		if (game->max_frames > 0 && ++frames >= game->max_frames)
			break ;
	}
	sdl_destroy(&sdl);
	return (0);
}
