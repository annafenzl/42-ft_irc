/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kill_cmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pguranda <pguranda@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 11:18:44 by pguranda          #+#    #+#             */
/*   Updated: 2023/04/12 16:58:07 by pguranda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/Server.hpp"
# include <algorithm>

# include <sstream>
# include <set>

//Implementation of a KILL command for IRC
void Server::kill_command(Request request)
{
	std::string response;
	
	std::string		user_to_kill = request.get_params()[0];
	User			dead_user;
	std::string		reason = request.get_params()[1];
	User			*user = request.get_user();
	
	if (!user->is_registered())
	{
		send_message(SERVER_NAME " 462 " + user->get_nickname() + " :Unauthorized command (not yet registered)", user->get_fd());
		return ;
	}
	if ((check_for_user(user_to_kill)) != _user_map.end())
	{
		dead_user = check_for_user(user_to_kill)->second;
	}
	else
		response = SERVER_NAME " 401 " + user_to_kill + " " + user_to_kill + " :No such nick/channel";
	if (!user->is_registered())
	{
		response = SERVER_NAME " 451 " + user->get_nickname() + " :You have not registered";
	}
	if (!user->is_operator())
	{
		response = SERVER_NAME " 481 " + user->get_nickname() + " :Permission Denied- You're not an IRC operator";
	}
	if (reason.empty())
	{
		response = SERVER_NAME " 461 " + user->get_nickname() + " :Not enough parameters";
	}
	if (!response.empty())
	{
		send_message(response, user->get_fd());
		return ;
	}
	if (dead_user.get_channel())
	{
		if (dead_user.get_channel () == NULL)
			return ;
		std::list<User *>::const_iterator userIt = std::find (dead_user.get_channel ()->getMembers (0).begin (), 
			dead_user.get_channel ()->getMembers (0).end (),
			&dead_user);
		dead_user.get_channel ()->getMembers (0).erase (userIt);
		dead_user.set_channel (NULL);
	}
	send_message( user_to_kill + " killed by " + user->get_nickname() + " (" + reason + ")", dead_user.get_fd());
	for (unsigned int i = 0; i < _fd_count; ++i)
	{
		if (_user_poll[i].fd == dead_user.get_fd())
		{
			std::cout << "USER " << dead_user.get_nickname() << " hung up on " << dead_user.get_fd() << std::endl;
			remove_from_poll(i);
			break;
		}
	}
	//Removing the dead_user from the channels, where he is in
// Removing the dead_user from the channels, where he is in

		
	_user_map.erase(dead_user.get_fd());
}