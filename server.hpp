/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afenzl <afenzl@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/12 16:55:23 by afenzl            #+#    #+#             */
/*   Updated: 2023/03/19 15:59:38 by afenzl           ###   ########.fr       */
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

# include "User.hpp"

# define MAXLINE 4096

// struct pollfd {
//                int   fd;         /* file descriptor */
//                short events;     /* requested events */ // bitmap of events we're interested in
//                short revents;    /* returned events */ // when poll() returns, bitmap of events that occurred
//            };

class Server
{
	
	private:
	int					_port;
	std::string			_password;
	
	int					_listening_socket;
	pollfd				_user_poll[SOMAXCONN];
	nfds_t				_fd_count;
	std::map<int, User> _user_map;

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

	void execute_command(User user, Request request);

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
			return "Error: Binding failed.";
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