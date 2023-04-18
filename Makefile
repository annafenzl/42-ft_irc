# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: katchogl <katchogl@student.42heilbronn.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/12 16:19:52 by afenzl            #+#    #+#              #
#    Updated: 2023/04/08 14:33:07 by katchogl         ###   ########.fr        #

#                                                                              #
# **************************************************************************** #

NAME = ircserv

CC = c++
CFLAGS = -Wall -Wextra -Werror -fsanitize=address -g -std=c++98 -MD 
RM := rm -f

SRC = main.cpp	$(addprefix src/, Server.cpp User.cpp Request.cpp Channel.cpp Bot.cpp) \
				$(addprefix src/cmds/, cap_cmd.cpp nick_cmd.cpp ping_cmd.cpp pass_cmd.cpp \
				user_cmd.cpp quit_cmd.cpp privmsg_cmd.cpp notice_cmd.cpp\
				oper_cmd.cpp kill_cmd.cpp globops_cmd.cpp \
				join_names_cmd.cpp list_cmd.cpp part_cmd.cpp topic_cmd.cpp who_cmd.cpp channel_mode_cmd.cpp showtime_bot_cmd.cpp kick_cmd.cpp)


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

run :
	@make
	./$(NAME) 6668 123

.PHONY: all clean fclean re
