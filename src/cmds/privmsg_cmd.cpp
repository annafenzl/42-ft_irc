/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg_cmd.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afenzl <afenzl@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 16:12:24 by afenzl            #+#    #+#             */
/*   Updated: 2023/03/24 14:18:01 by afenzl           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/Server.hpp"

# include <sstream>
# include <set>

/*
	function takes target string and splits it at ','
*/
std::set<std::string>	split_targets(std::string targets, std::string &duplicate)
{
	std::set<std::string>	splitted_targets;
	std::istringstream		line(targets);
	std::string				s;

	while (getline(line, s, ','))
	{
		// if s is already in the set
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

	sending to multiple targets:
		if one target is invalid, id doesn't send the message to anybody
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
		if (duplicate.empty() == false)
			response = SERVER_NAME " 407 " + user->get_nickname() + " " + duplicate + " :Duplicate recipients. No message delivered";
		
		else
		{
			for (std::set<std::string>::iterator it = targets.begin(); it != targets.end(); ++it)
			{
				// ERR_NOSUCHNICK
				// ERR_CANNOTSENDTOCHAN
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
