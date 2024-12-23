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
CFLAGS = -Wall -Wextra -Werror
RM = rm -rf

SRC = src/fractol.c \
src/mandelbrot.c \
src/my_mlx.c \
src/utils.c \
src/julia.c \
src/ft_atof_julia.c \

MLX = ./minilibx-linux

OBJS = $(SRC:.c=.o)
NAME = fractol

all : $(NAME)

$(NAME): $(OBJS)
	@make -C minilibx-linux/
	@make -C libft/
	@make -C ft_printf/
	@$(CC) $(OBJS) -Lmlx_linux -lmlx_Linux -L$(MLX) -lXext -lX11 -lm -lz -L libft/ -lft -L ft_printf/ -lftprintf -o $(NAME)

%.o : %.c
	@$(CC) $(CFLAGS) -I$(MLX) -Imlx_linux -O3 -c $< -o $@

clean :
	@make clean -C libft/
	@make clean -C ft_printf/
	@$(RM) $(OBJS)

fclean :
	@make fclean -C libft/
	@make fclean -C ft_printf/
	@$(RM) $(OBJS) $(NAME)

re : 
	@make fclean
	@make all

.PHONY: all clean fclean re
