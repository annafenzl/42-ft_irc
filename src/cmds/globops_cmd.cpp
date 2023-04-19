/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globops_cmd.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pguranda <pguranda@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 11:18:48 by pguranda          #+#    #+#             */
/*   Updated: 2023/04/18 23:00:16 by pguranda         ###   ########.fr       */

/*                                                                            */
/* ************************************************************************** */

# include "Server.hpp"

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
	//make a string with all the params
	std::string params;
	for (size_t i = 0; i < request.get_params().size(); i++)
	{
		params += request.get_params()[i];
		if (i != request.get_params().size() - 1)
			params += " ";
	}
	for(std::map<int, User>::const_iterator it = _user_map.begin(); it != _user_map.end(); ++it)
	{
		if (it->second.get_fd() != user->get_fd() && it->second.is_operator())
		{
			send_message( ":" + user->get_nickname() + "!" + user->get_nickname() + "@" + user->get_hostmask() + " GLOBOPS :" + params, it->second.get_fd());

		}
	}
}