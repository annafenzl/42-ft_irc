/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   oper_cmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pguranda <pguranda@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 18:13:39 by pguranda          #+#    #+#             */
/*   Updated: 2023/04/09 11:45:38 by pguranda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/Server.hpp"
# include <algorithm>

/*
	Command: OPER
	Parameters: <user> <password>
*/
void Server::oper_command(Request request)
{
	const std::string OPER_USERNAME = "admin";
	const std::string OPER_PASSWORD = "42";

	User *user = request.get_user();
	
	if (!user->is_registered())
	{
		send_message(SERVER_NAME " 462 " + user->get_nickname() + " :Unauthorized command (not yet registered)", user->get_fd());
		return ;
	}

	if (user->is_operator())
	{
		send_message(SERVER_NAME " 481 " + user->get_nickname() + " :Permission Denied- You're already an operator", user->get_fd());
		return;
	}
	// Check if enough parameters are provided
	if (request.get_params().size() < 2)
	{
		send_message(SERVER_NAME " 461 " + user->get_nickname() + " OPER :Not enough parameters", user->get_fd());
		return;
	}

	std::string username = request.get_params()[0];
	std::string password = request.get_params()[1];

	if (username == OPER_USERNAME && password == OPER_PASSWORD)
	{
		user->set_operator(true);
		send_message(SERVER_NAME " 381 " + user->get_nickname() + " :You are now an IRC operator", user->get_fd());
	}
	else
	{
		send_message(SERVER_NAME " 464 " + user->get_nickname() + " :Invalid username or password", user->get_fd());
	}
}
