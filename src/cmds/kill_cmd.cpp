/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kill_cmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pguranda <pguranda@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 11:18:44 by pguranda          #+#    #+#             */
/*   Updated: 2023/04/14 14:23:06 by pguranda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/Server.hpp"
# include <algorithm>

# include <sstream>
# include <set>

//Implementation of a KILL command for IRC
// void Server::kill_command(Request request)
// {
// 	std::string response;
	
// 	std::string		user_to_kill = request.get_params()[0];
// 	User			dead_user;
// 	std::string		reason = request.get_params()[1];
// 	User			*user = request.get_user();
	
// 	if (!user->is_registered())
// 	{
// 		send_message(SERVER_NAME " 462 " + user->get_nickname() + " :Unauthorized command (not yet registered)", user->get_fd());
// 		return ;
// 	}
// 	if ((check_for_user(user_to_kill)) != _user_map.end())
// 	{
// 		dead_user = check_for_user(user_to_kill)->second;
// 	}
// 	else
// 		response = SERVER_NAME " 401 " + user_to_kill + " " + user_to_kill + " :No such nick/channel";
// 	if (!user->is_registered())
// 	{
// 		response = SERVER_NAME " 451 " + user->get_nickname() + " :You have not registered";
// 	}
// 	if (!user->is_operator())
// 	{
// 		response = SERVER_NAME " 481 " + user->get_nickname() + " :Permission Denied- You're not an IRC operator";
// 	}
// 	if (reason.empty())
// 	{
// 		response = SERVER_NAME " 461 " + user->get_nickname() + " :Not enough parameters";
// 	}
// 	if (!response.empty())
// 	{
// 		send_message(response, user->get_fd());
// 		return ;
// 	}
// 	// if (dead_user.get_channel())
// 	// {
// 	// 	if (dead_user.get_channel () == NULL)
// 	// 		return ;
// 	// 	std::list<User *>::const_iterator userIt = std::find (dead_user.get_channel ()->getMembers (0).begin (), 
// 	// 		dead_user.get_channel ()->getMembers (0).end (),
// 	// 		&dead_user);
// 	// 	dead_user.get_channel ()->getMembers (0).erase (userIt);
// 	// 	dead_user.set_channel (NULL);
// 	// }
// 	send_message( user_to_kill + " killed by " + user->get_nickname() + " (" + reason + ")", dead_user.get_fd());
// 	for (unsigned int i = 0; i < _fd_count; ++i)
// 	{
// 		if (_user_poll[i].fd == dead_user.get_fd())
// 		{
// 			std::cout << "USER " << dead_user.get_nickname() << " hung up on " << dead_user.get_fd() << std::endl;
// 			remove_from_poll(i);
// 			break;
// 		}
// 	}
// 	//Removing the dead_user from the channels, where he is in
// 	// Removing the dead_user from the channels, where he is in
// 	// find latest channel in which user is, else stays NULL
// 	channelmap::iterator			channelIt = _channels.begin ();
// 	channelmap::iterator			channelItFound = _channels.end ();
// 	std::list<User *>::iterator		userIt;
// 	std::cout << "[DEBUG] searching for user: " + dead_user.get_nickname () << std::endl;
// 	// std::cout << "[DEBUG] user is in channel: " + dead_user.get_channel ()->getName () << std::endl;
// 	while (channelIt != _channels.end ())
// 	{
// 		std::cout << "[DEBUG] searching in channel: " + channelIt->first << std::endl;
// 		std::list<User *> members= channelIt->second.getMembers(0);
// 		std::cout << "[DEBUG] member[0] =" << members.front()->get_nickname() << std::endl;

// 		for (std::list<User *>::iterator it = members.begin(); it != members.end(); it++)
// 		{
// 			if ((*it)->get_nickname() == dead_user.get_nickname())
// 			{
// 				userIt = it;
// 				std::cout << "[DEBUG] user found in channel: " + channelIt->first << std::endl;
// 				channelItFound = channelIt;
// 				break ;
// 			}
// 		}
// 		channelIt++;
// 	}
// 	if (channelItFound != _channels.end ())
// 	{
// 		std::cout << "\033[0;36muser is now in channel: " + channelItFound->first + "\033[0m" << std::endl;
// 		dead_user.set_channel(NULL);
// 		channelItFound->second.getMembers(0).erase(userIt);
// 	}
// 	else
// 		std::cout << "\033[0;36muser is not in any channel\033[0m" << std::endl;
		
// 	_user_map.erase(dead_user.get_fd());

void Server::kill_command(Request request)
{
	std::string response;
	std::string user_to_kill = request.get_params()[0];
	std::string reason = request.get_params()[1];
	User* user = request.get_user();

	if (!user->is_registered()) {
		send_message(SERVER_NAME " 462 " + user->get_nickname() + " :Unauthorized command (not yet registered)", user->get_fd());
		return;
	}

	User* dead_user =  &check_for_user(user_to_kill)->second;
	if (!dead_user) {
		response = SERVER_NAME " 401 " + user_to_kill + " " + user_to_kill + " :No such nick/channel";
	}

	if (!user->is_operator()) {
		response = SERVER_NAME " 481 " + user->get_nickname() + " :Permission Denied- You're not an IRC operator";
	}

	if (reason.empty()) {
		response = SERVER_NAME " 461 " + user->get_nickname() + " :Not enough parameters";
	}

	if (!response.empty()) {
		send_message(response, user->get_fd());
		return;
	}

	std::string message = user_to_kill + " killed by " + user->get_nickname() + " (" + reason + ")";
	if (dead_user) {
		send_message(message, dead_user->get_fd());
		for (unsigned int i = 0; i < _fd_count; ++i) {
			if (_user_poll[i].fd == dead_user->get_fd()) {
				std::cout << "USER " << dead_user->get_nickname() << " hung up on " << dead_user->get_fd() << std::endl;
				remove_from_poll(i);
				break;
			}
		}
		for(channelmap::iterator it = _channels.begin(); it != _channels.end(); it++) 
		{
			std::list<User*>& members = it->second.getMembers(0);
			members.remove(dead_user);
		}
		_user_map.erase(dead_user->get_fd());
	} else 
	{
		send_message(message, user->get_fd());
	}
}
