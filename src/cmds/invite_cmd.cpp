/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite_cmd.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pguranda <pguranda@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 22:28:21 by pguranda          #+#    #+#             */
/*   Updated: 2023/04/19 22:35:37 by pguranda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "Server.hpp"

/************************************************
 *	usage:	/INVITE <nickname> <channel>
 ***********************************************/

void Server::invite_command(Request request)
{
	std::string nickname, channelName;
	User* invitedUser;
	channelmap::iterator it;

	if (request.get_params().size() < 2)
	{
		return send_message(request, RES_ERR_NEEDMOREPARAMS);
	}

	nickname = request.get_params()[0];
	channelName = request.get_params()[1];

	// Check if channel name is valid
	if (!Channel::isValidChannelName(channelName))
	{
		send_message(request, RES_ERR_BADCHANNAME);
		return;
	}

	// Check if user is registered
	if (!request.get_user()->is_registered())
	{
		send_message(request, RES_ERR_NOTREGISTERED_CHAN);
		return;
	}

	// Check if channel exists
	it = _channels.find(channelName);
	if (it == _channels.end())
	{
		send_message(request, RES_ERR_NOSUCHCHANNEL);
		return;
	}

	// Check if inviter is a member of the channel
	if (!it->second.isMember(request.get_user()))
	{
		send_message(request, RES_ERR_NOTONCHANNEL);
		return;
	}

	// Check if user to invite exists
	usermap::iterator userIt = check_for_user(nickname);
	if (userIt == _user_map.end())
	{
		send_message(request, RES_ERR_NOSUCHNICK);
		return;
	}
	invitedUser = &(userIt->second);

	// Check if invited user is already a member of the channel
	if (it->second.isMember(invitedUser))
	{
		send_message(request, RES_ERR_NOTONCHANNEL);
		return;
	}

	// Send invitation
	send_message(":" + request.get_user()->get_nickname() + "!" + request.get_user()->get_nickname() + "@" SERVER_NAME " INVITE " + invitedUser->get_nickname() + " :" + channelName, invitedUser->get_fd());
	send_message(request, RES_INVITED);
}
