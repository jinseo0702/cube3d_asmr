#ifndef PLATFORM_H
# define PLATFORM_H

# include "cub3d.h"

/*
** Implemented by exactly one backend (platform_sdl2.c or platform_mlx.c),
** selected at build time. Owns the window and the main loop: each frame it
** translates native key events into game->keys, calls game_update(), and
** presents game->fb to the screen.
*/
int	platform_run(t_game *game);

#endif
