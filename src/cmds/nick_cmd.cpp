/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick_cmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afenzl <afenzl@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 11:35:58 by afenzl            #+#    #+#             */
/*   Updated: 2023/03/20 16:02:22 by afenzl           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/Server.hpp"

bool	checkforbiddenchars(std::string password)
{
	for (unsigned int i = 0; i < std::string(FORBIDDEN_CHARS).size(); ++i)
	{
		if (password.find(FORBIDDEN_CHARS[i]) != std::string::npos)
		{
			std::cout << "password is |" << password << std::endl;
			std::cout << "forbidden char is " << FORBIDDEN_CHARS[i] << std::endl;
			return true;
		}
	}
	return false;
}

void	Server::check_login_complete(User *user)
{
	if (user->is_pass_provided() && user->get_nickname() != "*" && user->get_name() != "*" && user->is_registered() == false)
	{
		// RPL_WELCOME
		send_message(SERVER_NAME " 001 " + user->get_nickname() + " :Welcome to the Internet Relay Network " + user->get_nickname(), user->get_fd());
		user->set_registered(true);
	}
}

void	Server::nick_command(Request request)
{
	std::string response;
	User		*user = request.get_user();
	
	if (user->is_pass_provided() == false)
		response.append(SERVER_NAME " 462 " + user->get_nickname() + " :Please provide the server password with PASS first");
	
	// ERR_NONICKNAMEGIVEN
	else if (request.get_params().size() != 1)
		response = SERVER_NAME " 431 " + user->get_nickname() + " :Not one nickname given";

	//  ERR_ERRONEUSNICKNAME
	else if (checkforbiddenchars(request.get_params()[0]))
		response = SERVER_NAME " 432 " + user->get_nickname() + " " + request.get_params()[0] + " :Invalid character";

	else
	{
		// ERR_NICKNAMEINUSE 
		for (std::map<int,User>::iterator it = _user_map.begin(); it != _user_map.end(); ++it)
		{
			if (it->second.get_nickname() == request.get_params()[0])
			{
				response = SERVER_NAME " 433 " + user->get_nickname() + " " + request.get_params()[0] + " :Nickname is already in use";
				send_message(response, user->get_fd());
				return;
			}
		}
		std::cout << "prefix: " << user->get_prefix() << std::endl;
		response.append(user->get_prefix() + " NICK " + request.get_params()[0]);
		user->set_nickname(request.get_params()[0]);	
	}

	check_login_complete(user);
	
	send_message(response, user->get_fd());
}