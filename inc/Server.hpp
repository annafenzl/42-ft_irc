/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annafenzl <annafenzl@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/12 16:55:23 by afenzl            #+#    #+#             */
/*   Updated: 2023/04/24 00:22:46 by annafenzl        ###   ########.fr       */
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
# include <algorithm>
# include <sstream>
# include <utility>
# include <ctime>
# include <numeric>
# include <set>
# include "response.hpp"
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
	public:
	typedef std::map<std::string, Channel>	channelmap;
	typedef std::map<int, User>				usermap;

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
	Server(char *port, char *password);

	// -------------- Getters ----------------------
	int					get_port();
	std::string			get_password();
	const channelmap	&getChannels( void ) const;

	Channel *find_channel(std::string channel_name);
	

	// -------------- Methods ----------------------
	void run();
	void setup_socket();

	void new_client();
	void client_request(int index);

	void add_to_poll(int user_fd);
	void remove_from_poll(int user_fd);

	void handle_command(char* cmd, int user_fd);
	void execute_command(Request request);
	void remove_user(User *user, std::string string);
	
	void					check_login_complete(User *user);
	usermap::iterator		check_for_user(std::string nickname);
	std::set<std::string>	split_targets(std::string targets, std::string &duplicate);

	// --- commands
	void cap_command(Request request);
	void ping_command(Request request);
	void nick_command(Request request);
	void user_command(Request request);
	void pass_command(Request request);
	void privmsg_command(Request request);
	void quit_command(Request request);
	void join_names_command (Request request);
	void list_command (Request request);
	void topic_command (Request request);
	void part_command (Request request);
	void who_command (Request request);
	void mode_command (Request request);
	void oper_command(Request request);
	void kill_command(Request request);
	void notice_command(Request request);
	void globops_command(Request request);
	void showtime_bot_command(Request request);
	void kick_command(Request request);
	void invite_command(Request request);
	
	static void send_message(std::string, int fd);
	static void send_message(Request req, t_res err);

	void send_names_list(Request &request, Channel &channel);
	static void broadcast (std::string message, User* user, Channel& channel);

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

typedef Server::usermap		usermap;
typedef Server::channelmap	channelmap;

#endif