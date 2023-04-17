/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globops_cmd.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afenzl <afenzl@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 11:18:48 by pguranda          #+#    #+#             */
/*   Updated: 2023/04/17 15:48:31 by afenzl           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/Server.hpp"
# include <algorithm>
# include <iterator>

//Implementation of a GLOBOPS command for IRC
void Server::globops_command(Request request)
{
	std::string	response;
	User		*user = request.get_user();
	
	if (!user->is_registered())
	{
		response = SERVER_NAME " 462 " + user->get_nickname() + " :Unauthorized command (not yet registered)";
	}
	else if (!user->is_operator())
	{
		response = SERVER_NAME " 481 " + user->get_nickname() + " :Permission Denied- You're not an IRC operator";
	}
	else if (request.get_params().size() == 0)
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
			send_message(SERVER_NAME " GLOBOPS " + user->get_nickname() + " : " + request.get_params()[0], it->second.get_fd());
		}
	}
}