/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg_cmd.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pguranda <pguranda@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 16:12:24 by afenzl            #+#    #+#             */
/*   Updated: 2023/04/19 14:34:43 by pguranda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Server.hpp"

# include <sstream>
# include <set>
# include <ctime>
# include <time.h>

/*
	takes target string and splits it at ','
	edits passed &duplicate string, if the target it already in the set
*/

std::set<std::string>	split_targets(std::string targets, std::string &duplicate)
{
	std::set<std::string>	splitted_targets;
	std::istringstream		line(targets);
	std::string				s;

	while (getline(line, s, ','))
	{
		if (false == splitted_targets.insert(s).second)
		{
			duplicate = s;
			break;
		}
	}

	return splitted_targets;
}

/*
	Command: PRIVMSG
	Parameters: <msgtarget{,msgtarget}> <text to be sent>

	PRIVMSG is used to send private messages between users, as well as to
	send messages to channels. <msgtarget> is usually the nickname of
	the recipient of the message, or a channel name.

	not sending to multiple targets:
		if one target is invalid
		if targets a duplicate

	   Numeric Replies:
		 ERR_NORECIPIENT - 1
		 ERR_NOTEXTTOSEND - 1
		ERR_CANNOTSENDTOCHAN - 0 -- too much
		ERR_NOTOPLEVEL - NA
		ERR_WILDTOPLEVEL - NA
		ERR_TOOMANYTARGETS - 1
		ERR_NOSUCHNICK - 1
		RPL_AWAY - 0
*/
void	Server::privmsg_command(Request request)
{
	std::string				response;
	User					*user = request.get_user();
	usermap::iterator		recipient;

	if (!user->is_registered())
		response = SERVER_NAME " 462 " + user->get_nickname() + " :Unauthorized command (not yet registered)";

	// ERR_NORECIPIENT
	else if (request.get_params().size() == 0 )
		response = SERVER_NAME " 411 " + user->get_nickname() + " :No recipient given PRIVSMG";

	// ERR_NOTEXTTOSEND
	else if (request.get_params().size() == 1)
		response = SERVER_NAME " 412 " + user->get_nickname() + " :No text to send";

	else
	{
		std::string				duplicate;
		std::set<std::string>	targets = split_targets(request.get_params()[0], duplicate);
	
		// ERR_TOOMANYTARGETS
		// Returned to a client which is attempting to send a
		//   PRIVMSG/NOTICE using the user@host desti nation format
		//   and for a user@host which has several occurrences.
		if (duplicate.empty() == false)
			response = SERVER_NAME " 407 " + user->get_nickname() + " " + duplicate + " :Duplicate recipients. No message delivered";
		else
		{
			std::set<std::string>::const_iterator it;
			for (it = targets.begin(); it != targets.end(); ++it)
			{
				// ERR_CANNOTSENDTOCHAN
				// std::cout << "available channels: "<< std::endl;
				for (channelmap::iterator it = _channels.begin(); it != _channels.end(); ++it)
					std::cout << it->first << ", ";
				std::cout << std::endl;
				// ERR_NOSUCHNICK
				if (check_for_user(*it) == _user_map.end() && _channels.find(*it) == _channels.end())
				{
					response = SERVER_NAME " 401 " + user->get_nickname()+ " " + *it + " :No such nick/channel";
					break;
				}
			}
			if (response.empty())
			{
				for (it = targets.begin(); it != targets.end(); ++it)
				{
					// send to user
					if ((recipient = check_for_user(*it)) != _user_map.end())
						send_message(user->get_prefix() + " PRIVMSG " + *it + " :" + request.get_params()[1], recipient->first);
					// send to all other users in channel
					else
					{
						const std::list<User *> &member_list = _channels.find(*it)->second.getMembers();
						if (member_list.empty())
							continue ;
						for (std::list<User *>::const_iterator user_it = member_list.begin(); user_it != member_list.end(); ++user_it)
						{
							if (*user_it != user)
								send_message(user->get_prefix() + " PRIVMSG " + *it + " :" + request.get_params()[1], (*user_it)->get_fd());
						}
					}
				}
				return;
			}
		}
	}

	send_message(response, user->get_fd());
}