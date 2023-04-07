# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pguranda <pguranda@student.42heilbronn.de> +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/12 16:19:52 by afenzl            #+#    #+#              #
#    Updated: 2023/04/07 18:15:25 by pguranda         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

CC = c++
CFLAGS = -Wall -Wextra -Werror -fsanitize=address -g -std=c++98 -MD 
RM := rm -f

SRC = main.cpp $(addprefix src/, Server.cpp User.cpp Request.cpp Channel.cpp) \
	$(addprefix src/cmds/, cap_cmd.cpp nick_cmd.cpp ping_cmd.cpp pass_cmd.cpp \
		user_cmd.cpp quit_cmd.cpp privmsg_cmd.cpp notice_cmd.cpp join_cmd.cpp oper_cmd.cpp)

OBJ = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) -pthread -o $(NAME)
	@echo "\033[0;32mplease run the program with ./ircserv <port> <passwort> \033[0m"

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re