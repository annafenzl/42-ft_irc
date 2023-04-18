/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   notice_cmd.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pguranda <pguranda@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 15:22:07 by annafenzl         #+#    #+#             */
/*   Updated: 2023/04/17 10:55:55 by pguranda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/Server.hpp"

# include <sstream>
# include <set>

std::set<std::string> split_targets(std::string targets, std::string &duplicate);

void	Server::notice_command(Request request)
{
	User *user = request.get_user();
	usermap::iterator recipient;

	// Do not process the command if the user is not registered
	if (!user->is_registered())
	{
		send_message(SERVER_NAME " 462 " + user->get_nickname() + " :Unauthorized command (not yet registered)", user->get_fd());
		return ;
	}
	// Do not process the command if no recipient or text is provided
	if (request.get_params().size() < 2)
		return;

	std::string duplicate;
	std::set<std::string> targets = split_targets(request.get_params()[0], duplicate);

	// Do not send the message if there is a duplicate recipient
	if (duplicate.empty())
	{
	// Send the message to all recipients
		for (std::set<std::string>::iterator it = targets.begin(); it != targets.end(); ++it)
		{
			if ((recipient = check_for_user(*it)) != _user_map.end())
			{
				send_message(user->get_prefix() + " NOTICE " + *it + " :" + request.get_params()[1], recipient->first);
			}
			else
			{
				const std::list<User *> &member_list = _channels.find(*it)->second.getMembers();
				for (std::list<User *>::const_iterator user_it = member_list.begin(); user_it != member_list.end(); ++user_it)
				{
					if (*user_it != user)
					send_message(user->get_prefix() + " NOTICE " + *it + " :" + request.get_params()[1], (*user_it)->get_fd());
				}
			}
		}
	}
	return;
}
