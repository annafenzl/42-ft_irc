/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg_cmd.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pguranda <pguranda@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 16:12:24 by afenzl            #+#    #+#             */
/*   Updated: 2023/04/23 15:24:38 by pguranda         ###   ########.fr       */
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

std::set<std::string>	Server::split_targets(std::string targets, std::string &duplicate)
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
							//check if member_list contains the user
						std::list<User*>::const_iterator user_it = std::find(member_list.begin(), member_list.end(), user);
						if (user_it == member_list.end())
						{
							// If the user is not a member of the channel
							send_message(user->get_prefix() + " 482 " + user->get_nickname() + " " + *it + " :You're not on that channel", user->get_fd());
							return;
						}
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