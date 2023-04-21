/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick_cmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pguranda <pguranda@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 21:54:36 by pguranda          #+#    #+#             */
/*   Updated: 2023/04/21 14:56:54 by pguranda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "../../inc/Server.hpp"
#include <algorithm>
#include <sstream>
#include <set>
#include <iterator>

// Helper function to split a string by a delimiter
std::vector<std::string> split(const std::string& str, char delimiter)
{
	std::vector<std::string> tokens;
	std::stringstream ss(str);
	std::string token;

	while (std::getline(ss, token, delimiter))
		tokens.push_back(token);
	return tokens;
}

void Server::kick_command(Request request)
{
	std::vector<std::string> params = request.get_params();
	User* user = request.get_user();
	std::string response;

	if (!user->is_registered()) 
	{
		send_message(SERVER_NAME " 462 " + user->get_nickname() + " :Unauthorized command (not yet registered)", user->get_fd());
		return;
	}
	if (params.size() < 2)
	{
		response = SERVER_NAME " 461 " + user->get_nickname() + " KICK :Not enough parameters";
		send_message(response, user->get_fd());
		return;
	}
	std::vector<std::string> channels = split(params[0], ',');
	std::vector<std::string> users_to_kick = split(params[1], ',');
	std::string comment = (params.size() > 2) ? params[2] : user->get_nickname();

	for (std::vector<std::string>::size_type i = 0; i < channels.size(); ++i)
	{
		Channel *channel = find_channel(channels[i]);
		if (!channel)
		{
			response = SERVER_NAME " 403 " + channels[i] + " :No such channel";
			send_message(response, user->get_fd());
			continue;
		}
		if (!channel->isOp(user))
		{
			response = SERVER_NAME " 482 " + channels[i] + " :You're not a channel operator";
			send_message(response, user->get_fd());
			continue;
		}
		
		for (std::vector<std::string>::size_type j = 0; j < users_to_kick.size(); ++j)
		{
			//If there is more than one channel, we take pairs e.g. KICK #chan1,#chan2 user1,user2 - kick user1 from #chan1 and user2 from #chan2 
			if (channels.size() != 1)
				j = i;
			User* target_user = &check_for_user(users_to_kick[j])->second;
			if (!target_user)
			{
				response = SERVER_NAME " 401 " + users_to_kick[j] + " :No such nick";
				send_message(response, user->get_fd());
				continue;
			}

			if (!channel->getMember(target_user->get_nickname()))
			{
				response = SERVER_NAME " 441 " + users_to_kick[j] + " " + channels[i] + " :They aren't on that channel";
				send_message(response, user->get_fd());
				continue;
			}
			std::string kick_message = "KICK " + channels[i] + " " + users_to_kick[j] + " :" + comment;
			channel->removeMember(&check_for_user(users_to_kick[j])->second);
			send_message(kick_message, target_user->get_fd());
			for (std::list<User*>::const_iterator userIt = channel->getMembers().begin(); userIt != channel->getMembers().end(); ++userIt)
			{
				send_message(kick_message, (*userIt)->get_fd());
			}
			//If there is more than one channel, we take pairs e.g. KICK #chan1,#chan2 user1,user2 - kick user1 from #chan1 and user2 from #chan2
			if (channels.size() != 1)
				break;
		}
		if (channel->getMembers().empty())
		{
			_channels.erase(channel->getName());
		}
	}
}
