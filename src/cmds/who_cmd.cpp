/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   who_cmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katchogl <katchogl@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 02:40:53 by katchogl          #+#    #+#             */
/*   Updated: 2023/04/20 19:55:07 by katchogl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Server.hpp"

/** /WHO when no args is provided: display all connected allUsers to the server;
 * 	/WHO when an argument (#username) is provided;
 * 	/WHO when an argument (#channel) is provided;
**/
void Server::who_command( Request request )
{
	std::map<std::string, User *>			allUsers;
	channelmap::iterator		channelIt;
	usermap::iterator			userIt;
	std::list<User *>::const_iterator			channelUserIt;
	std::map<std::string, User *>::const_iterator		allUsersIt;
	std::string					info;

	if (request.get_params ().size () < 1)
	{
		userIt = _user_map.begin ();
		while (userIt != _user_map.end ())
		{
			allUsers.insert (std::make_pair (userIt->second.get_nickname (), &userIt->second));
			userIt++;
		}
	}
	else if (Channel::isValidChannelName (request.get_params ()[0]))
	{
		channelIt = _channels.find (request.get_params ()[0]);
		request.set_channel_name (request.get_params ()[0]);
		if (channelIt == _channels.end ())
			return (send_message (request, RES_ERR_NOSUCHCHANNEL));
		channelUserIt = channelIt->second.getMembers ().begin ();
		while (channelUserIt != channelIt->second.getMembers ().end ())
		{
			allUsers.insert (std::make_pair ((*channelUserIt)->get_nickname (), *channelUserIt));
			channelUserIt++;
		}
	}
	else
	{
		userIt = check_for_user (request.get_params ()[0]);
		request.set_info (request.get_params ()[0]);
		if (userIt == _user_map.end ())
			return (send_message (request, RES_ERR_NOSUCHNICK));
		allUsers.insert (std::make_pair (userIt->second.get_nickname (), &userIt->second));
	}
	allUsersIt = allUsers.begin ();
	while (allUsersIt != allUsers.end ())
	{
		// :<server> 352 <client> <channel> <username> <host> <server> <nick> <H|G>[*][@|+] :<hopcount> <real name>
		info = " " + allUsersIt->second->get_name ()
			+ " " + allUsersIt->second->get_hostmask ()
			+ " " + SERVER_NAME // target user's server is connected to: SERVER_NAME by default
			+ " " + allUsersIt->second->get_nickname ();
		
		// user status and modes
		if (request.get_params ().size () > 0
			&& Channel::isValidChannelName (request.get_params ()[0])
			&& channelIt->second.isOp (allUsersIt->second))
			info += " @";
			
		info += std::string (" :0") // hopcount: 0 by default
			+ " " + allUsersIt->second->get_fullname ();
		request.set_info (info);
		send_message (request, RES_RPL_WHOREPLY);
		allUsersIt++;
	}
	send_message (request, RES_RPL_ENDOFWHO);
	return ;
}
