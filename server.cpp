/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annafenzl <annafenzl@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 00:13:32 by annafenzl         #+#    #+#             */
/*   Updated: 2023/03/14 21:57:49 by annafenzl        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Server.hpp"


// -------------- Constructor ------------------
Server::Server(char **argv)
{
	// PORTS WITH 878whatever are still valid!!!!
	long	port = strtol(argv[1], NULL, 0);
	if (port < 1 || port > 65535)  // portnumbers up until 1024 are reserved!!
		throw IncorrectPortNumber();
	_port = (int) port;
	_password = argv[2];
	// are there other passwords that are invalid
	if (_password.empty())
		throw InvalidPassword();
}

// -------------- Getters ----------------------
int	Server::get_port()
{
	return _port;
}

std::string	Server::get_password()
{
	return _password;
}

// -------------- Methods ----------------------

void Server::run()
{
	setup_socket();

	while (true)
	{
		if (poll(_user_poll, _fd_count, -1) == -1) // -1 is waiting for ever but i can specify the timeout
			throw PollFailedError();

		unsigned int		current_size = _fd_count;
		char				buff[MAXLINE];
		int					read_bytes;
		
		for (int i = 0; i < current_size; i++)
		{
			try{
				if (_user_poll[i].revents & POLLIN)
				{
					if (_user_poll[i].fd == _listening_socket)
						accept_user();
					else
					{
						memset(buff, 0, MAXLINE);
						read_bytes = recv(_user_poll[i].fd, buff, MAXLINE, 0);
						if (read_bytes == 0) // should mean the remote side did close connection
							remove_from_poll(i);
						else
							std::cout << "\nfile descriptor " << _user_poll[i].fd << " is ready to read!" << std::endl;
						buff[read_bytes] = '\0';
						printf("|%s|\n", buff);
					}
				}
			} catch (std::exception& e) {
				std::cout << e.what() << std::endl;
			}
		}
		close(_listening_socket);
	}
}

void Server::setup_socket()
{
	struct sockaddr_in	_servaddr;

	if ( (_listening_socket = socket(PF_INET, SOCK_STREAM, 0)) == -1)
		throw CreateSocketError();

	std::cout << "binding on port: " << _listening_socket << std::endl;

	memset(&_servaddr, 0, sizeof(_servaddr));
	_servaddr.sin_family = AF_INET; //Address Family Internet
	_servaddr.sin_port = htons(_port);
	_servaddr.sin_addr.s_addr = inet_addr("0.0.0.0");

	int enable = 1;
	if (setsockopt(_listening_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) == -1)
		throw SetSocketOptionError();

	if (bind(_listening_socket, (sockaddr*)&_servaddr, sizeof(_servaddr)) == -1)
		throw BindSocketError();
	
	if (listen(_listening_socket, SOMAXCONN) == -1) // SOMAXCONN is the max number of connections allowed in the incomming queue
		throw ListenSocketError();

//	prepare user poll
	memset(_user_poll, 0, sizeof(_user_poll));
	_fd_count = 1;
	_user_poll[0].fd = _listening_socket;
	_user_poll[0].events = POLLIN;
}

void Server::accept_user()
{
	int	user_fd;
	struct sockaddr_in	clientaddr;
	socklen_t			addrlen;
			
	if ((user_fd = accept(_listening_socket, (sockaddr *)&clientaddr, &addrlen)) == -1)
		throw AcceptSocketError();
		
	add_to_poll(user_fd);
}

void Server::add_to_poll(int user_fd)
{
	_user_poll[_fd_count].fd = user_fd;
	_user_poll[_fd_count].events = POLLIN;

	++_fd_count;
}

void Server::remove_from_poll(int index)
{
	std::cout << "Removing " << _user_poll[index].fd << " from poll!" << std::endl;
	close(_user_poll[index].fd);
	_user_poll[index].fd = _user_poll[_fd_count-1].fd;
	_user_poll[index].events = _user_poll[_fd_count-1].events;

	--_fd_count;
}

