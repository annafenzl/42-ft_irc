/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   notice_cmd.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katchogl <katchogl@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 15:22:07 by annafenzl         #+#    #+#             */
/*   Updated: 2023/04/18 23:20:26 by pguranda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Server.hpp"

std::set<std::string> split_targets(std::string targets, std::string &duplicate);

void	Server::notice_command(Request request)
{
	User					*user = request.get_user();

	if (!user->is_registered() || request.get_params().size() < 2)
	{
		return;
	}

	std::string				duplicate;
	std::set<std::string>	targets = split_targets(request.get_params()[0], duplicate);

	std::string message;
	for (size_t i = 1; i < request.get_params().size(); ++i)
	{
		message += request.get_params()[i];
		if (i < request.get_params().size() - 1)
		{
			message += " ";
		}
	}

	std::set<std::string>::const_iterator it;
	for (it = targets.begin(); it != targets.end(); ++it)
	{
		usermap::iterator recipient = check_for_user(*it);
		channelmap::iterator channel = _channels.find(*it);

		if (recipient != _user_map.end())
		{
			send_message(user->get_prefix() + " NOTICE " + *it + " :" + message, recipient->first);
		}
		else if (channel != _channels.end())
		{
			const std::list<User *> &member_list = channel->second.getMembers();
			for (std::list<User *>::const_iterator user_it = member_list.begin(); user_it != member_list.end(); ++user_it)
			{
				if (*user_it != user)
				{
					send_message(user->get_prefix() + " NOTICE " + *it + " :" + message, (*user_it)->get_fd());
				}
			}
		}
	}
}
