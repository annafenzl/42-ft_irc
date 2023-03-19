/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afenzl <afenzl@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 00:13:32 by annafenzl         #+#    #+#             */
/*   Updated: 2023/03/19 17:18:48 by afenzl           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../inc/Server.hpp"

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
			try{
				if (_user_poll[i].revents & POLLIN)
				{
					if (_user_poll[i].fd == _listening_socket)
						new_client();
					else
						client_request(i);
				}
			} catch (std::exception& e) {
				std::cerr << "\033[0;31m" << e.what() << "\033[0m" << '\n';
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
	
	char	host[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &clientaddr.sin_addr, host, INET_ADDRSTRLEN);
	_user_map.insert(std::make_pair(user_fd, User(user_fd, host)));
		
	add_to_poll(user_fd);
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
		handle_command(buff, sender_fd);
}

void Server::add_to_poll(int user_fd)
{
	if (_fd_count >= SOMAXCONN)
		throw FdPollFullError();
	
	_user_poll[_fd_count].fd = user_fd;
	_user_poll[_fd_count].events = POLLIN;

	++_fd_count;
}

void Server::remove_from_poll(int index)
{
	close(_user_poll[index].fd);
	_user_poll[index].fd = _user_poll[_fd_count-1].fd;
	_user_poll[index].events = POLLIN;
	_user_poll[_fd_count - 1].fd = -1;
	
	--_fd_count;
}

void Server::handle_command(char* cmd, int user_fd)
{
	User	user = _user_map.find(user_fd)->second;

	user.append_buff(cmd);
	for (int end_pos = user.buff.find(END_SEQUENCE); end_pos != std::string::npos ; end_pos = user.buff.find(END_SEQUENCE))
	{
		std::string part = user.buff.substr(0, end_pos);
		user.buff.erase(0, end_pos + 2);
		
		Request request(part, &user);
		request.print();
		execute_command(request);
	}
}

void Server::execute_command( Request request)
{
	std::string cmd = request.get_cmd();
	
	if (cmd == "CAP")
		cap_command(request);
	else if (cmd == "PING")
		ping_command(request);
	else if (cmd == "NICK")
		nick_command(request);
	// else if (cmd == "PASS")
	// else if (cmd == "USER")
	// else if (cmd == "MODE")
}

void Server::cap_command(Request request)
{
	std::string response;

	if (request.get_params()[0] == "LS")
	{
		response = (SERVER_NAME " CAP * LS :End of CAP LS negotiation");
		send_message(response, request.get_user()->get_fd());
	}	
}

void Server::ping_command(Request request)
{
	std::string response(SERVER_NAME);

	if (request.get_params().size() == 0)
		response.append(" 409 " + request.get_user()->get_nickname() + " :No origin specified");
	else
		response.append(" PONG " SERVER_NAME);
	
	send_message(response, request.get_user()->get_fd());
}

void Server::nick_command(Request request)
{
	std::string response(SERVER_NAME);
	
	if (request.get_params().size() == 0)
		response.append(" 431 " + request.get_user()->get_nickname() + " :No nickname given");
	
	for (std::map<int,User>::iterator it = _user_map.begin(); it != _user_map.end(); ++it)
		if (it->second.get_nickname() == request.get_params()[0])
			response.append(" 433 " + request.get_user()->get_nickname() + " " + request.get_params()[0] + " :Nickname is already in use");
			
	
	send_message(response, request.get_user()->get_fd());
}

void Server::send_message(std::string message, int fd)
{
	send(fd, message.append(END_SEQUENCE).c_str(), message.size(), 0);
}
