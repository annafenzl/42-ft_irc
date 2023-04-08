/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globops_cmd.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pguranda <pguranda@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 11:18:48 by pguranda          #+#    #+#             */
/*   Updated: 2023/04/08 14:57:26 by pguranda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/Server.hpp"
# include <algorithm>
# include <iterator>

//Implementation of a GLOBOPS command for IRC
void Server::globops_command(Request request)
{
	std::string response;
	
	std::string		message = request.get_params()[0];
	User			*user = request.get_user();
	
	if (!user->is_registered())
	{
		response = SERVER_NAME " 451 " + user->get_nickname() + " :You have not registered";
	}
	if (!user->is_operator())
	{
		response = SERVER_NAME " 481 " + user->get_nickname() + " :Permission Denied- You're not an IRC operator";
	}
	if (message.empty())
	{
		response = SERVER_NAME " 461 " + user->get_nickname() + " :Not enough parameters";
	}
	if (!response.empty())
	{
		send_message(response, user->get_fd());
		return ;
	}
	for(std::map<int, User>::const_iterator it = _user_map.begin(); it != _user_map.end(); ++it)
	{
		if (it->second.get_fd() != user->get_fd() && it->second.is_operator())
		{
			send_message(SERVER_NAME " GLOBOPS " + user->get_nickname() + " : " + message, it->second.get_fd());
		}
	}
}