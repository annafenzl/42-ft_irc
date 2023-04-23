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
CFLAGS = -Wall -Wextra -Werror -g -std=c++98 -I./inc #-fsanitize=address
RM := rm -f
CDEPS = $(patsubst %, inc/%.hpp, Bot Channel response Request Server User);

SRC = main.cpp	$(addprefix src/, Server.cpp User.cpp Request.cpp Channel.cpp Bot.cpp send_message.cpp) \
				$(addprefix src/cmds/, cap_cmd.cpp nick_cmd.cpp ping_cmd.cpp pass_cmd.cpp \
				user_cmd.cpp quit_cmd.cpp privmsg_cmd.cpp notice_cmd.cpp \
				oper_cmd.cpp kill_cmd.cpp globops_cmd.cpp \
				join_names_cmd.cpp list_cmd.cpp part_cmd.cpp topic_cmd.cpp who_cmd.cpp mode_cmd.cpp showtime_bot_cmd.cpp kick_cmd.cpp invite_cmd.cpp)

OBJ = $(SRC:.cpp=.o)

%.o: %.cpp $(CDEPS)
	$(CC) $(CFLAGS) -c $< -o $@ 

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	@echo "\033[0;32mplease run the program with ./ircserv <port> <passwort> \033[0m"

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)
	@rm -rf *.d
	@rm -rf src/*.d
	@rm -rf inc/*.d
	@rm -rf src/cmds/*.d
	@rm -rf *.dmg

re: fclean all

run :
	@make
	./$(NAME) 6668 123

get_client:
	@echo "\033[0;32mDownloading and launching KVIrc client...\033[0m"
	@curl -o KVIrc-5.0.0.dmg ftp://ftp.kvirc.net/pub/kvirc/5.0.0/binary/macosx/KVIrc-5.0.0.dmg
	@open KVIrc-5.0.0.dmg
 
.PHONY: all clean fclean re
