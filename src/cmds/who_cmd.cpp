/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   who_cmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katchogl <katchogl@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 02:40:53 by katchogl          #+#    #+#             */
/*   Updated: 2023/04/19 03:02:30 by katchogl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Server.hpp"

/** /WHO when no args is provided: display all connected allUsers to the server;
 * 	/WHO when an argument (#username) is provided;
 * 	/WHO when an argument (#channel) is provided;
**/
void Server::who_command( Request request )
{
	std::vector<User *>			allUsers;
	channelmap::iterator		channelIt;
	usermap::iterator			userIt;
	std::list<User *>::const_iterator			channelUserIt;
	std::vector<User *>::const_iterator		allUsersIt;

	if (request.get_params ().size () < 1)
	{
		userIt = _user_map.begin ();
		while (userIt != _user_map.end ())
		{
			allUsers.insert (allUsers.end (), &userIt->second);
			userIt++;
		}
	}
	else if (Channel::isValidChannelName (request.get_params ()[0]))
	{
		channelIt = _channels.find (request.get_params ()[0]);
		request.set_channel_name (request.get_params ()[0]);
		if (channelIt == _channels.end ())
		{
			send_message (request, RES_ERR_NOSUCHCHANNEL);
			return ;
		}
		channelUserIt = channelIt->second.getMembers ().begin ();
		while (channelUserIt != channelIt->second.getMembers ().end ())
		{
			allUsers.insert (allUsers.end (), *channelUserIt);
			channelUserIt++;
		}
	}
	else
	{
		userIt = check_for_user (request.get_params ()[0]);
		request.set_info (request.get_params ()[0]);
		if (userIt == _user_map.end ())
		{
			send_message (request, RES_ERR_NOSUCHNICK);
			return ;
		}
		allUsers.insert (allUsers.end (), &userIt->second);
	}
	allUsersIt = allUsers.begin ();
	while (allUsersIt != allUsers.end ())
	{
		// :<server> 352 <client> <channel> <username> <host> <server> <nick> <H|G>[*][@|+] :<hopcount> <real name>
		request.set_info (
			+ " " + (*allUsersIt)->get_name ()
			+ " " + (*allUsersIt)->get_hostmask ()
			+ " " + SERVER_NAME // server user is connected to, SERVER_NAME by default
			+ " " + (*allUsersIt)->get_nickname ()
			+ " *" // user status and modes
			+ ":0" // hopcount
			+ " " + (*allUsersIt)->get_fullname ());
		send_message (request, RES_RPL_WHOREPLY);
		allUsersIt++;
	}
	send_message (request, RES_RPL_ENDOFWHO);
	return ;
}
