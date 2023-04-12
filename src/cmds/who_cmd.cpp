/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   who_cmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katchogl <katchogl@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 02:40:53 by katchogl          #+#    #+#             */
/*   Updated: 2023/04/12 05:01:10 by katchogl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/Server.hpp"

void Server::who_command( Request request )
{
	std::string					info;
	std::vector<User *>			users;
	channelmap::iterator		channelIt;
	usermap::iterator			userIt;
	std::list<User *>::const_iterator			channelUserIt;
	std::vector<User *>::const_iterator		usersIt;

	if (request.get_params ().size () < 1)
	{
		userIt = _user_map.begin ();
		while (userIt != _user_map.end ())
		{
			users.insert (users.end (), &userIt->second);
			userIt++;
		}
	}
	else if (Channel::isValidChannelName (request.get_params ()[0]))
	{
		channelIt = _channels.find (request.get_params ()[0]);
		if (channelIt == _channels.end ())
		{
			send_message (request, EXIT_ERR_NOSUCHCHANNEL,
				request.get_params ()[0]);
			return ;
		}
		channelUserIt = channelIt->second.getMembers ().begin ();
		while (channelUserIt != channelIt->second.getMembers ().end ())
		{
			users.insert (users.end (), *channelUserIt);
			channelUserIt++;
		}
	}
	else
	{
		userIt = check_for_user (request.get_params ()[0]);
		if (userIt == _user_map.end ())
		{
			send_message (request, EXIT_ERR_NOSUCHNICK,
				request.get_params ()[0]);
			return ;
		}
	}
	usersIt = users.begin ();
	while (usersIt != users.end ())
	{
		info = (*usersIt)->get_nickname ();
		send_message (request, EXIT_RPL_WHOREPLY, info);
		usersIt++;
	}
	send_message (request, EXIT_RPL_ENDOFWHO, "");
	return ;
}
