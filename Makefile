# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jinseo <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/31 16:11:14 by jinseo            #+#    #+#              #
#    Updated: 2024/07/31 19:30:09 by jinseo           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Wextra -Werror -g
# CFLAGS = -g
RM = rm -rf

SRC = src2/main.c \
      src2/event.c \
      src2/key_handlers.c \
      src2/player_movement.c \
      src2/ray_casting.c \
      src2/ray_dda.c \
      src2/render_walls.c \
      src2/render_fc.c \
      src2/texture_utils.c \
      src2/texture_render.c \
      src2/line_draw.c \
      src2/parsing_init.c \
      src2/parsing_color.c \
      src2/parsing_map_check.c \
      src2/parsing_map_data.c \
      src2/flood_fill.c \

MLX = ./minilibx-linux

OBJS = $(SRC:.c=.o)
NAME = cub3D

all : $(NAME)

$(NAME): $(OBJS)
	@make -C minilibx-linux/
	@make -C libft/
	@$(CC) $(OBJS) -no-pie -Lmlx_linux -lmlx_Linux -L$(MLX) -lXext -lX11 -lm -lz -L libft/ -lft -o $(NAME)

%.o : %.c
	@$(CC) $(CFLAGS) -I$(MLX) -Imlx_linux -O3 -c $< -o $@

clean :
	@make clean -C libft/
	@$(RM) $(OBJS)

fclean :
	@make fclean -C libft/
	@$(RM) $(OBJS) $(NAME)

re : 
	@make fclean
	@make all

.PHONY: all clean fclean re