# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yingzhan <yingzhan@student.42berlin.de>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/01 14:06:20 by yingzhan          #+#    #+#              #
#    Updated: 2025/08/08 10:03:31 by yingzhan         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex
NAME_BONUS = pipex_bonus
CC = cc
CFLAGS = -Wall -Wextra -Werror
RM = rm -f
INCLUDES = -I./ -I./libft/includes/

SRCS = main.c parse.c utils.c
BONUS_SRCS = main_bonus.c parse_bonus.c utils_bonus.c
OBJS = $(SRCS:.c=.o)
BONUS_OBJS = $(BONUS_SRCS:.c=.o)

LIBFT = ./libft/libft.a

all: $(NAME)
$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)

bonus: $(NAME_BONUS)
$(NAME_BONUS): $(BONUS_OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(BONUS_OBJS) $(LIBFT) -o $(NAME_BONUS)

$(LIBFT):
	make -C libft

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) $(OBJS) $(BONUS_OBJS)
	make -C libft clean

fclean: clean
	$(RM) $(NAME) $(NAME_BONUS)
	make -C libft fclean

re: fclean all

re_bonus: fclean bonus

.PHONY: all clean fclean re bonus re_bonus

