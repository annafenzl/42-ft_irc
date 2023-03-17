/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afenzl <afenzl@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 00:13:32 by annafenzl         #+#    #+#             */
/*   Updated: 2023/03/17 16:13:59 by afenzl           ###   ########.fr       */
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
	if (setsockopt(_listening_socket, SOL_SOCKET, SO_REUSEADDR, (const char*)&enable, sizeof(int)) == -1)
		throw SetSocketOptionError();

	fcntl(_listening_socket, F_SETFL, O_NONBLOCK);
	if (_listening_socket == -1)
		throw CreateSocketError();

	if (bind(_listening_socket, (sockaddr*)&_servaddr, sizeof(_servaddr)) == -1)
		throw BindSocketError();

	if (listen(_listening_socket, SOMAXCONN) == -1) // SOMAXCONN is the max number of connections allowed in the incomming queue
		throw ListenSocketError();

//	prepare user poll
	memset(_user_poll, 0, sizeof(_user_poll));
	_user_poll[0].fd = _listening_socket;
	_user_poll[0].events = POLLIN;
	_fd_count = 1;
}

void Server::run()
{
	setup_socket();

	while (true)
	{
		if (poll(_user_poll, _fd_count, -1) == -1) // -1 is waiting for ever but i can specify the timeout
			throw PollFailedError();

		unsigned int		current_size = _fd_count;
		for (unsigned int i = 0; i < current_size; i++)
		{
			// std::cout << _user_poll[1].fd << ", " << _user_poll[1].events << ", " << _user_poll[1].revents << std::endl;
			try{
				if (_user_poll[i].revents & POLLIN)
				{
					if (_user_poll[i].fd == _listening_socket)
						new_client();
					else
						client_request(i);
				}
			} catch (std::exception& e) {
				std::cout << e.what() << std::endl;
			}
		}
	}
	close(_listening_socket);
}

void Server::new_client()
{
	int					user_fd;
	socklen_t			addrlen;
	struct sockaddr_in	clientaddr;
			
	if ((user_fd = accept(_listening_socket, (sockaddr *)&clientaddr, &addrlen)) == -1)
		throw AcceptSocketError();
		
	add_to_poll(user_fd);
	// std::cout << inet_ntoa(((struct sockaddr_in*)&clientaddr)->sin_addr) << " on socket " << user_fd << std::endl;
}

void Server::client_request(int index)
{
	char				buff[MAXLINE];
	int					read_bytes;
	int					sender_fd = _user_poll[index].fd;
	
	memset(buff, 0, MAXLINE);
	read_bytes = recv(sender_fd, buff, MAXLINE, 0);
	
	if (read_bytes <= 0)
	{
		if (read_bytes == 0)
			std::cout << "user(fd) " << sender_fd << " hung up" << std::endl;
		else
			throw RecieveMessageFailed();
		remove_from_poll(index);
	}
	else
	{
		printf("recv: |%s|\n", buff);
	}
}

void Server::add_to_poll(int user_fd)
{
	if (_fd_count >= SOMAXCONN)
		throw FdPollFullError();
	
	_user_poll[_fd_count].fd = user_fd;
	_user_poll[_fd_count].events = POLLIN;

	++_fd_count;
	std::cout << "in poll is:\n";
	for (int i = 0; i < _fd_count; ++i)
		std::cout << _user_poll[i].fd << ", " << _user_poll[i].events << ", " << _user_poll[i].revents << std::endl;
}

void Server::remove_from_poll(int index)
{
	std::cout << "Removing " << _user_poll[index].fd << " from poll!" << std::endl;
	close(_user_poll[index].fd);
	_user_poll[index].fd = _user_poll[_fd_count-1].fd;
	_user_poll[index].events = POLLIN;
	_user_poll[_fd_count - 1].fd = -1;
	
	--_fd_count;
	std::cout << "in poll is:\n";
	for (int i = 0; i < _fd_count; ++i)
		std::cout << _user_poll[i].fd << ", " << _user_poll[i].events << ", " << _user_poll[i].revents << std::endl;
}

