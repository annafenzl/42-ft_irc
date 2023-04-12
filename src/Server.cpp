/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katchogl <katchogl@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 00:13:32 by annafenzl         #+#    #+#             */
/*   Updated: 2023/04/12 20:40:54 by katchogl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../inc/Server.hpp"

// -------------- Constructor ------------------
Server::Server(char **argv)
{
	char	*check;
	long	port = strtol(argv[1], &check, 0);
	if (port < 1 || port > 65535 || *check != '\0')  // portnumbers up until 1024 are reserved!!
		throw IncorrectPortNumber();
	_port = (int) port;
	_password = argv[2];
	// are there other passwords that are invalid 
	if (_password.empty())
		throw InvalidPassword();

	// safe time of creation
	std::time_t current_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	_time_of_creation = std::ctime(&current_time);
	_time_of_creation.pop_back();
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
						add_client();
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

void Server::add_client()
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
	std::cout << "\nnew client on fd " << user_fd << "!" << std::endl;
}

void Server::client_request(int index)
{
	char				buff[MAXLINE];
	int					read_bytes;
	int					sender_fd = _user_poll[index].fd;
	
	memset(buff, 0, MAXLINE);
	read_bytes = recv(sender_fd, buff, MAXLINE, 0);

	std::cout << std::endl;
	
	if (read_bytes <= 0)
	{
		if (read_bytes == 0)
		{
			std::cout << "user(fd) " << sender_fd << " hung up" << std::endl;
			_user_map.erase(sender_fd);
		}
		else
		{
			std::cerr << std::strerror(errno) << '\n';
			throw RecieveMessageFailed();
		}
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
	User	*user = &_user_map.find(user_fd)->second;

	user->append_buff(cmd);
	for (int end_pos = user->buff.find(END_SEQUENCE); end_pos != std::string::npos; end_pos = user->buff.find(END_SEQUENCE))
	{
		std::string part = user->buff.substr(0, end_pos);
		user->buff.erase(0, end_pos + 2);
		
		Request request(part, user);
		request.print();
		execute_command(request);
	}
}

void Server::execute_command( Request request)
{
	std::string cmd = request.get_cmd();

	if ((cmd == "JOIN" || cmd == "PART")
		&& !request.get_user ()->is_registered ())
	{
		send_message (request, EXIT_ERR_NOTREGISTERED, "");
		return ;
	}
		
	if (cmd == "CAP")
		cap_command(request);
	else if (cmd == "PING")
		ping_command(request);
	else if (cmd == "PASS")
		pass_command(request);
	else if (cmd == "NICK")
		nick_command(request);
	else if (cmd == "USER")
		user_command(request);
	else if (cmd == "PRIVMSG")
		privmsg_command(request);
	else if (cmd == "QUIT")
		quit_command(request);
	else if (cmd == "JOIN" || cmd == "NAMES")
		join_names_command (request);
	else if (cmd == "LIST")
		list_command (request);
	else if (cmd == "TOPIC")
		topic_command (request);
	else if (cmd == "PART")
		part_command (request);
	else if (cmd == "WHO")
		who_command (request);
	// else if (cmd == "MODE")
	else
		send_message(SERVER_NAME " 421 " + request.get_user()->get_nickname() + " " + cmd + " :Unknown command", request.get_user()->get_fd());
}

void Server::send_message(std::string message, int fd)
{
	std::cout << "RESPONSE IS <" << message << ">" << std::endl;
	send(fd, message.append(END_SEQUENCE).c_str(), message.size(), 0);
}

/*
	searches for User in the map, returns a iterator to the User, if not found _user_map.end()
*/
std::map<int,User>::iterator Server::check_for_user(std::string nickname)
{
	for (std::map<int,User>::iterator it = _user_map.begin(); it != _user_map.end(); ++it)
		{
			if (it->second.get_nickname() == nickname)
			{
				return it;
			}
		}
	return _user_map.end();
}

const Server::channelmap &Server::getChannels( void ) 
	const { return (_channels); }

void Server::send_message(Request req, t_exit err, std::string info)
{
	std::string mes;
    std::ostringstream stream;

	stream << static_cast<int>(err);
	mes.append (SERVER_NAME).append (" " + stream.str ()
		+ " " + req.get_user ()->get_nickname () + " :");

	switch (err)
	{
		case EXIT_CHANNEL_JOINED:
			send_message (":" + req.get_user ()->get_nickname () + "!" 
				+ req.get_user ()->get_name () + "@" + SERVER_NAME + " JOIN "
				+ info, req.get_user ()->get_fd ());
			return ;
		case EXIT_ERR_NEEDMOREPARAMS:
			mes.append ("need more parameters");
			break;
		case EXIT_ERR_TOOMANYCHANNELS:
			mes.append ("Too many channels");
			break;
		case EXIT_ERR_ALREADY_JOINED:
			mes.append ("already joined: " + info);
			break;
		case EXIT_ERR_INVALID_CHANNEL_NAME:
			mes.append ("invalid channel name");
			break;
		case EXIT_ERR_NOSUCHCHANNEL:
			mes.append ("no such channel: " + info);
			break;
		case EXIT_RPL_ENDOFWHO:
			mes.append ("end of /WHO");
			break ;
		case EXIT_RPL_ENDOFNAMES:
			mes.append ("end of /NAMES");
			break ;
		case EXIT_RPL_LISTSTART:
			mes.append ("start of /LIST");
			break ;
		case EXIT_RPL_LISTEND:
			mes.append ("end of /LIST");
			break ;
		case EXIT_INFO_ONLY:
		case EXIT_RPL_WHOREPLY:
		case EXIT_RPL_LIST:
		case EXIT_RPL_NAMREPLY:
			mes.append (info);
			break;
		case EXIT_LEFT_CHANNEL:
			send_message (":" + req.get_user ()->get_nickname () + "!" 
				+ req.get_user ()->get_name () + "@" + SERVER_NAME + " PART "
				+ info, req.get_user ()->get_fd ());
			return ;
		case EXIT_ERR_NOTREGISTERED:
			mes.append ("You are not yet registered");
			break ;
		case EXIT_ERR_ALREADYREGISTERED:
			mes.append ("You are already registered");
			break ;
		case EXIT_ERR_NOSUCHNICK:
			mes.append ("no such nickname");
			break ;
		case EXIT_ERR_NOTONCHANNEL:
			mes.append ("not on channel");
			break ;
		default:
		    std::ostringstream stream2;
			stream2 << static_cast<int>(err);
			mes.append ("error code not found: " + stream2.str ());
	}
	send_message (mes, req.get_user ()->get_fd ());
}
