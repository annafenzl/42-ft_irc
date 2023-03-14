# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: annafenzl <annafenzl@student.42.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/12 16:19:52 by afenzl            #+#    #+#              #
#    Updated: 2023/03/14 01:12:37 by annafenzl        ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_irc

CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98
RM := rm -f

SRC = main.cpp server.cpp
OBJ = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) -pthread -o $(NAME)
	@echo "\033[0;32mplease run the program with <port> <passwort> \033[0m"

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re