/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pguranda <pguranda@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid Date        by              +#+  #+#    #+#             */
/*   Updated: 2023/04/12 15:27:49 by pguranda         ###   ########.fr       */
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
# include "exit.hpp"
# include "User.hpp"
# include "Request.hpp"
# include "Channel.hpp"

# include "Bot.hpp"
# define MAXLINE 4096
# define END_SEQUENCE "\r\n"
# define SERVER_NAME ":ircfornow.com"
# define FORBIDDEN_CHARS "!@#$%^&*()+={}[];,:\"\t'<>."

//OPER Credentials
# define OPER_PASS "42"
# define OPER_LOG "admin"

class Server
{
	typedef std::map<int, User>		usermap;
	typedef std::map<std::string, Channel>	channelmap;

	private:
		int					_port;
		std::string			_password;
		
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
	void notice_command(Request request);
	void oper_command(Request request);
	void kill_command(Request request);
	void globops_command(Request request);
	
	void send_message(std::string, int fd);

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

		t_exit channel_manager( Request req );
		t_exit send_message(Request req, t_exit err, std::string info);
		const channelmap &getChannels( void ) const;
		
};
#endif