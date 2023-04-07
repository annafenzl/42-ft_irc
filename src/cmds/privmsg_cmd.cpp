/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg_cmd.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pguranda <pguranda@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 16:12:24 by afenzl            #+#    #+#             */
/*   Updated: 2023/04/07 18:04:34 by pguranda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/Server.hpp"

# include <sstream>
# include <set>

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
		//   PRIVMSG/NOTICE using the user@host destination format
		//   and for a user@host which has several occurrences.
		if (duplicate.empty() == false)
			response = SERVER_NAME " 407 " + user->get_nickname() + " " + duplicate + " :Duplicate recipients. No message delivered";
		else
		{
			for (std::set<std::string>::iterator it = targets.begin(); it != targets.end(); ++it)
			{
				// ERR_CANNOTSENDTOCHAN
				
				// ERR_NOSUCHNICK
				if ((recipient = check_for_user(*it)) == _user_map.end())
				{
					response = SERVER_NAME " 401 " + user->get_nickname()+ " " + *it + " :No such nick/channel";
					break;
				}
			}
			if (response.empty())
			{
				for (std::set<std::string>::iterator it = targets.begin(); it != targets.end(); ++it)
					send_message(user->get_prefix() + " PRIVMSG " + *it + " :" + request.get_params()[1], check_for_user(*it)->first);
				return;
			}
		}
	}

	// ERR_NOTOPLEVEL	
		// 413   "<mask> :No toplevel domain specified"
	// ERR_WILDTOPLEVEL
		// 414 "<mask> :Wildcard in toplevel domain"
	// RPL_AWAY if the user is currently away
	
	send_message(response, user->get_fd());
}
