/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pguranda <pguranda@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/12 16:55:23 by afenzl            #+#    #+#             */
/*   Updated: 2023/04/18 22:56:02 by pguranda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef SERVER_HPP
# define SERVER_HPP
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <errno.h>
# include <string.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <sys/wait.h>
# include <poll.h>
# include <signal.h>
# include <string>
# include <iostream>
# include <fcntl.h>
# include <map>
# include <sstream>
# include <utility>
# include <ctime>
# include <numeric>
# include "exit.hpp"
# include "User.hpp"
# include "Request.hpp"
# include "Channel.hpp"

# include "Bot.hpp"
# define MAXLINE 4096
# define END_SEQUENCE "\r\n"
# define SERVER_NAME ":ircfornow.com"
# define FORBIDDEN_CHARS "!@#$%^&*()+={}[];,:\"\t'<>."
# define VERSION "1.0"

//OPER Credentials
# define OPER_PASS "42"
# define OPER_LOG "admin"

class Server
{
	typedef std::map<int, User>				usermap;
	typedef std::map<std::string, Channel>	channelmap;

	private:
		int					_port;
		std::string			_password;
		std::string			_time_of_creation;
		
		int					_listening_socket;
		pollfd				_user_poll[SOMAXCONN];
		nfds_t				_fd_count;
		usermap				_user_map;
		channelmap			_channels;

	public:
	// -------------- Constructor ------------------
	Server(char **argv);

	// -------------- Getters ----------------------
	int get_port();
	const channelmap &getChannels( void ) const;

	Channel *find_channel(std::string channel_name);
	
	std::string get_password();

	// -------------- Methods ----------------------
	void run();
	void setup_socket();

	void new_client();
	void client_request(int index);

	void add_to_poll(int user_fd);
	void remove_from_poll(int user_fd);

	void handle_command(char* cmd, int user_fd);
	void execute_command(Request request);
	void remove_user(User *user);
	void remove_user(User *user, std::string &string);

	
	void				check_login_complete(User *user);
	usermap::iterator	check_for_user(std::string nickname);

	// --- commands
	void cap_command(Request request);
	void ping_command(Request request);
	void nick_command(Request request);
	void user_command(Request request);
	void pass_command(Request request);
	void privmsg_command(Request request);
	// void join_command(Request request);
	void quit_command(Request request);
	void join_names_command (Request request);
	void list_command (Request request);
	void topic_command (Request request);
	void part_command (Request request);
	void who_command (Request request);
	void channel_mode_command (Request request);
	void oper_command(Request request);
	void kill_command(Request request);
	void notice_command(Request request);
	void globops_command(Request request);
	void send_message(std::string, int fd);
	void send_message(Request req, t_exit err, std::string info);
	void showtime_bot_command(Request request);
	void kick_command(Request request);
	
	void send_names_list(Request &request, Channel &channel);
	void broadcast_join_message(User* user, Channel& channel);

	// -------------- Exceptions -------------------
	class IncorrectPortNumber: public std::exception {
		const char * what() const throw() {
			return "Error: Provided portnumber is incorrect.";
		}
	};

	class InvalidPassword: public std::exception {
		const char * what() const throw() {
			return "Error: Provided password is invalid.";
		}
	};

	class CreateSocketError: public std::exception {
		const char * what() const throw() {
			return "Error: Creating socket failed.";
		}
	};

	class SetSocketOptionError: public std::exception {
		const char * what() const throw() {
			return "Error: Setting socket options failed.";
		}
	};

	class BindSocketError: public std::exception {
		const char * what() const throw() {
			return "Error: Adding User failed, too many Users connected.";
		}
		};

		class ListenSocketError: public std::exception {
		const char * what() const throw() {
			return "Error: Listening failed.";
		}
	};

	class AcceptSocketError: public std::exception {
		const char * what() const throw() {
			return "Error: Accepting failed.";
		}
	};

	class RecieveMessageFailed: public std::exception {
		const char * what() const throw() {
			return "Error: Recieving the message failed.";
		}
	};

	class FdPollFullError: public std::exception {
	const char * what() const throw() {
		return "Error: Adding User failed, too many Users connected.";
	}
	};

	class PollFailedError: public std::exception {
		const char * what() const throw() {
			return "Error: The poll() function failed.";
		}
	};
};
#endif