/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annafenzl <annafenzl@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 00:13:32 by annafenzl         #+#    #+#             */
/*   Updated: 2023/04/24 00:28:04 by annafenzl        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../inc/Server.hpp"

// -------------- Constructor ------------------
Server::Server(char *port, char *password)
{
	// parse port
	char	*end;
	long	port_num = strtol(port, &end, 0);
	if (port_num < 1 || port_num > 65535 || end[0] != '\0')
		throw IncorrectPortNumber();
	_port = (int) port_num;
	
	// parse password
	if (_password.empty())
		throw InvalidPassword();
	_password = password; 

	// set time of creation
	time_t now = time(0);
	_time_of_creation = std::string(ctime(&now));
	_time_of_creation = _time_of_creation.substr(0, _time_of_creation.size() - 1);
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

const Server::channelmap &Server::getChannels( void ) const
{
	return (_channels);
}

// -------------- Methods ----------------------
// should i set .events to POLOUT too to check if sending is possible
void Server::setup_socket()
{
	struct sockaddr_in	_servaddr;

	if ( (_listening_socket = socket(PF_INET, SOCK_STREAM, 0)) == -1)
		throw CreateSocketError();

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
	unsigned int current_size = _fd_count;
	
	setup_socket();

	while (true)
	{
		if (poll(_user_poll, _fd_count, -1) == -1)
			throw PollFailedError();

		current_size = _fd_count;
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

	std::cout << "new client on fd " << user_fd << "!" << std::endl;
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
			std::cout << "user(fd) " << sender_fd << " hung up!" << std::endl;
		else
			throw RecieveMessageFailed();
		remove_user(&_user_map.find(sender_fd)->second, "disconnected");
	}
	else
		handle_command(buff, sender_fd);
}

void Server::remove_user(User *user, std::string reason)
{
	std::list<std::string>	empty_channels;
	int						user_fd = user->get_fd();

	// remove from all channels && inform their members
	for (channelmap::iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		if (it->second.isMember(user))
		{
			if (it->second.remove(user))
				empty_channels.insert(empty_channels.end(), it->second.getName());
			for (std::list<User *>::const_iterator iter = it->second.getMembers(0).begin(); iter != it->second.getMembers(0).end(); iter ++)
			{
					send_message(user->get_prefix() + " QUIT :" + reason, (*iter)->get_fd());
			}
		}
	}

	// remove empty channels
	for (std::list<std::string>::const_iterator it = empty_channels.begin(); it != empty_channels.end(); ++it)
	{
		_channels.erase(*it);
	}
	

	// remove from poll array
	for (unsigned int i = 0; i < _fd_count; ++i)
	{
		if (_user_poll[i].fd == user_fd)
		{
			remove_from_poll(i);
			break ;
		}
	}

	// remove from user map
	_user_map.erase(user->get_fd());
}

void Server::handle_command(char* cmd, int user_fd)
{
	User	*user = &_user_map.find(user_fd)->second;

	user->append_buff(cmd);
	for (size_t end_pos = user->buff.find(END_SEQUENCE);end_pos != 512 && end_pos != std::string::npos ; end_pos = user->buff.find(END_SEQUENCE))
	{
		std::string part = user->buff.substr(0, end_pos);
		user->buff.erase(0, end_pos + 2);
		Request request(part, user);
		request.print();
		execute_command(request);
		if (_user_map.find(user_fd) == _user_map.end())
			break;
	}
}

void Server::execute_command( Request request)
{
	std::string cmd = request.get_cmd();
		
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
	else if (cmd == "GLOBOPS")
		globops_command(request);
	else if (cmd == "SHOWTIME")
		showtime_bot_command(request);
	else if (cmd == "LIST")
		list_command (request);
	else if (cmd == "TOPIC")
		topic_command (request);
	else if (cmd == "OPER")
		oper_command(request);
	else if (cmd == "PART")
		part_command (request);
	else if (cmd == "WHO")
		who_command (request);
	else if (cmd == "MODE")
		mode_command (request);
	else if (cmd == "KILL")
		kill_command(request);
	else if (cmd == "KICK")
		kick_command(request);
	else if (cmd == "NOTICE")
		notice_command(request);
	else if (cmd == "INVITE")
		invite_command(request);
	else
		send_message(SERVER_NAME " 421 " + request.get_user()->get_nickname() + " " + cmd + " :Unknown command", request.get_user()->get_fd());
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

/*
	searches for User in the map, returns a iterator to the User, if not found _user_map.end()
*/
usermap::iterator Server::check_for_user(std::string nickname)
{
	for (usermap::iterator it = _user_map.begin(); it != _user_map.end(); ++it)
		{
			if (it->second.get_nickname() == nickname)
			{
				return it;
			}
		}
	return _user_map.end();
}

void Server::send_message(std::string message, int fd)
{
	std::cout << "RESPONSE IS <" << message << ">" << std::endl;
	send(fd, message.append(END_SEQUENCE).c_str(), message.size(), 0);
}

Channel * Server::find_channel(std::string channel_name)
{
	for (channelmap::iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		if (it->second.getName() == channel_name)
		{
			return &it->second;
		}
	}
	return NULL;
}

// move this back to CHannel.cpp
bool Channel::execMode(char mode, char sign, std::string param, const Server & server, Request request)
{
	User	*user;

	// invite only channel or only ops can change topic (don't need to be exec'ed, only edit'ed)
	if (mode == 'i' || mode == 't')
		return (true);

	// operator
	else if (mode == 'o')
	{
		if (param.empty())
		{
			server.send_message(request, RES_ERR_NEEDMOREPARAMS);
			return (false);
		}
		user = getMember (param);
		if (request.get_user() == user && sign == '-')
			return (false);
		request.set_info (param); 
		if (user == NULL)
		{
			server.send_message (request, RES_ERR_USERNOTINCHANNEL);
			return (false);
		}
		if (sign == '+' && isOp (user))
		{
			server.send_message (request, RES_ERR_ALREADYANOPERATOR);
			return (false);	
		}
		else if (sign == '+')
			insertOp (user);
		else if (!isOp (user))
		{
			server.send_message (request, RES_ERR_NOTANOPERATOR);
			return (false);
		}
		else
			removeOp (user);
		return (true);
	}
	// key
	else if (mode == 'k')
	{
		if (sign == '-')
		{
			setPassword("*");
			return true;
		}
		else
		{
			setPassword(param);
			return true;
		}
	}
	// limit
	else if (mode == 'l')
	{
		if (sign == '-')
		{
			setLimit(-1);
			return true;
		}
		else
		{
			int update_limit = strtol(param.c_str(), NULL, 10);
			if (update_limit >= static_cast<int>(_members.size()))
			{
				_limit = update_limit;
				std::cout << "set limit to " << _limit << std::endl;
				return true;
			}
		}
	}
	return false;
}

void Channel::removeOp( User * op )
{
	_ops.remove(op);
	if (_ops.size () == 0 && !_members.empty())
	{
		insertOp(*_members.begin());
		// ... and broadcast
		Server::broadcast (":" + std::string (SERVER_NAME)
					+ " MODE"
					+ " " + _name
					+ " +o " + (*_members.begin())->get_nickname ()
					, NULL, *this);
	}
}
