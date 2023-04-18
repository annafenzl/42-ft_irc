/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   who_cmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katchogl <katchogl@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 02:40:53 by katchogl          #+#    #+#             */
/*   Updated: 2023/04/18 23:08:07 by katchogl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Server.hpp"

/** /WHO when no args is provided: display all connected allUsers to the server;
 * 	/WHO when an argument (#username) is provided;
 * 	/WHO when an argument (#channel) is provided;
**/
void Server::who_command( Request request )
{
	std::string					info;
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
		if (channelIt == _channels.end ())
		{
			send_message (request, EXIT_ERR_NOSUCHCHANNEL,
				request.get_params ()[0]);
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
		if (userIt == _user_map.end ())
		{
			send_message (request, EXIT_ERR_NOSUCHNICK,
				request.get_params ()[0]);
			return ;
		}
	}
	allUsersIt = allUsers.begin ();
	while (allUsersIt != allUsers.end ())
	{
		info = (*allUsersIt)->get_nickname () 
			+ " " + (*allUsersIt)->get_name ()
			+ " " + (*allUsersIt)->get_hostmask ()
			+ SERVER_NAME
			+ " " + (*allUsersIt)->get_fullname ();
		send_message (request, EXIT_RPL_WHOREPLY, info);
		allUsersIt++;
	}
	send_message (request, EXIT_RPL_ENDOFWHO, "");
	return ;
}
