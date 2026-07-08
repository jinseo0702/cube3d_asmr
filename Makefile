NAME       := cub3D
CC         := cc
CFLAGS     := -Wall -Wextra -Werror -O2 -Iinclude -Ilibft
UNAME      := $(shell uname -s)

# Backend selection: SDL2 on macOS, MinilibX (X11) on Linux.
# Override with `make BACKEND=sdl2` or `make BACKEND=mlx`.
ifeq ($(UNAME),Darwin)
BACKEND    ?= sdl2
else
BACKEND    ?= mlx
endif

CORE_SRCS  := src/main.c \
              src/parser/parse_scene.c \
              src/parser/parse_config.c \
              src/parser/parse_map.c \
              src/parser/validate_map.c \
              src/parser/read_lines.c \
              src/engine/game.c \
              src/engine/player.c \
              src/render/raycast.c \
              src/render/framebuffer.c \
              src/render/texture.c \
              src/render/minimap.c

ifeq ($(BACKEND),sdl2)
SRCS           := $(CORE_SRCS) src/platform/platform_sdl2.c
BACKEND_CFLAGS := $(shell sdl2-config --cflags)
BACKEND_LIBS   := $(shell sdl2-config --libs)
BACKEND_DEP    :=
else
MLX_DIR        := minilibx-linux
MLX_URL        := https://github.com/42Paris/minilibx-linux.git
SRCS           := $(CORE_SRCS) src/platform/platform_mlx.c
BACKEND_CFLAGS := -I$(MLX_DIR)
BACKEND_LIBS   := -L$(MLX_DIR) -lmlx -lXext -lX11
BACKEND_DEP    := $(MLX_DIR)/libmlx.a
endif

LIBFT      := libft/libft.a
OBJ_DIR    := build
OBJS       := $(SRCS:%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

$(NAME): $(BACKEND_DEP) $(LIBFT) $(OBJS)
	$(CC) $(OBJS) $(BACKEND_LIBS) -Llibft -lft -lm -o $(NAME)

$(OBJ_DIR)/%.o: %.c include/cub3d.h include/platform.h
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(BACKEND_CFLAGS) -c $< -o $@

$(LIBFT):
	$(MAKE) -C libft

ifeq ($(BACKEND),mlx)
$(MLX_DIR)/Makefile:
	git clone --depth 1 $(MLX_URL) $(MLX_DIR)

$(MLX_DIR)/libmlx.a: $(MLX_DIR)/Makefile
	$(MAKE) -C $(MLX_DIR)
endif

test: $(NAME)
	sh tests/run_tests.sh

clean:
	$(MAKE) clean -C libft
	rm -rf $(OBJ_DIR)

fclean: clean
	$(MAKE) fclean -C libft
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re test
