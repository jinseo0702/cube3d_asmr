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
# CFLAGS = -Wall -Wextra -Werror
CFLAGS = -g
RM = rm -rf

SRC = src/main.c \
src/draw.c \
src/parsing.c \
src/parsing_2.c \

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
