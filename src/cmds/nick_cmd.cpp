/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick_cmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pguranda <pguranda@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 11:35:58 by afenzl            #+#    #+#             */
/*   Updated: 2023/04/23 15:24:15 by pguranda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Server.hpp"

/*
	forbidden chars are "!@#$%^&*()+={}[];,:\"\t'<>." 
*/
bool	checkforbiddenchars(std::string nickname)
{
	for (unsigned int i = 0; i < std::string(FORBIDDEN_CHARS).size(); ++i)
	{
		if (nickname.find(FORBIDDEN_CHARS[i]) != std::string::npos)
		{
			std::cout << "nickname is |" << nickname << std::endl;
			std::cout << "forbidden char is " << FORBIDDEN_CHARS[i] << std::endl;
			return true;
		}
	}
	return false;
}

/*
	if password is provided, user and nickname are present, but user is not registered
	--> sets registered to true
*/
void	Server::check_login_complete(User *user)
{
	std::string	nickname = user->get_nickname();
	int			fd = user->get_fd();

	if (user->is_pass_provided() && nickname != "*" && user->get_name() != "*" && user->is_registered() == false)
	{
		// RPL_WELCOME
		send_message(SERVER_NAME " 001 " + nickname + " :Welcome to the Internet Relay Network " + nickname + "!" + user->get_name() + "@" + user->get_hostmask(), fd);
		// RPL_YOURHOST
		send_message(SERVER_NAME " 002 " + nickname + " :Your host is " SERVER_NAME " running version " VERSION, fd);
		// RPL_CREATED
		send_message(SERVER_NAME " 003 " + nickname + " :This server was created " + _time_of_creation, fd);
		user->set_registered(true);
	}
}

/*
	Command: NICK
	Parameters: <nickname>
	The NICK command is used to give user a nickname or change the existing
	one.
*/

void	Server::nick_command(Request request)
{
	std::string	response;
	User		*user = request.get_user();
	
	if (user->is_pass_provided() == false)
		response = SERVER_NAME " 464 " + user->get_nickname() + " :Please provide the server password with PASS first";
	
	// ERR_NONICKNAMEGIVEN
	else if (request.get_params().size() != 1)
		response = SERVER_NAME " 431 " + user->get_nickname() + " :Not one nickname given";

	//  ERR_ERRONEUSNICKNAME
	else if (checkforbiddenchars(request.get_params()[0]))
		response = SERVER_NAME " 432 " + user->get_nickname() + " " + request.get_params()[0] + " :Invalid character";
	
	// ERR_NICKNAMEINUSE 
	else if (check_for_user(request.get_params()[0]) != _user_map.end())
		response = SERVER_NAME " 433 " + user->get_nickname() + " " + request.get_params()[0] + " :Nickname is already in use";
		
	else
	{
		response = (user->get_prefix() + " NICK " + request.get_params()[0]);

		user->set_nickname(request.get_params()[0]);
		for ( std::map < std::string , Channel * > ::iterator it = user->getChannels(0).begin(); it != user->getChannels(0).end(); it++)
		{
			broadcast(response, NULL, *(it->second));
		}
	}
	check_login_complete(user);
	send_message(response, user->get_fd());
}