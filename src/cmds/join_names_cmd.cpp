/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_names_cmd.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katchogl <katchogl@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 12:21:43 by katchogl          #+#    #+#             */
/*   Updated: 2023/04/12 19:11:33 by katchogl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/Server.hpp"

/************************************************
 *	! /JOIN /NAMES handler !
 *
 *	usage:	/JOIN #channel1,#channel2
 *			/NAMES #channel1,#channel2
 ***********************************************/

void Server::join_names_command( Request request )
{
	size_t								pos;
	std::string 						channelNames;
	std::string 						channelName;
	std::string							info;
	channelmap::iterator				it;
	std::list<User *>::const_iterator	userIt;

	if (request.get_params ().size () < 1
		&& request.get_cmd () == "NAMES"
		&& request.get_user ()->get_channel () != NULL)
		request.get_params ().insert (request.get_params ().end (),
				request.get_user ()->get_channel ()->getName ());
	
	if (request.get_params ().size () < 1)
	{
		send_message (request, EXIT_ERR_NEEDMOREPARAMS, "");
		return ;
	}
	
	channelNames = request.get_params ()[0];
	while (true)
	{
		if (channelNames.empty ())
			break;
		
		pos = channelNames.find (",");
		channelName = channelNames.substr (0, pos);
		
		if (pos < channelNames.length ())
			channelNames = channelNames.substr (pos + 1);
		else
			channelNames = "";

		if (!Channel::isValidChannelName (channelName))
		{
			send_message (request, EXIT_ERR_INVALID_CHANNEL_NAME, channelName);
			continue ;
		}
		it = _channels.find (channelName);
		if (it == _channels.end () && request.get_cmd () == "JOIN")
		{
			_channels.insert (std::make_pair (channelName,
				Channel (channelName)));
			it = _channels.find (channelName);
		}
		else if (it == _channels.end () && request.get_cmd () == "NAMES")
		{
			send_message (request, EXIT_ERR_NOSUCHCHANNEL, channelName);
			continue ;
		}

		if (request.get_cmd () == "JOIN")
		{
			request.get_user ()->set_channel (&it->second);
			if (it->second.getMember (request.get_user ()) == NULL)
			{
				it->second.insert (request.get_user ());
				send_message (request, EXIT_CHANNEL_JOINED, channelName);
			}
			else
				send_message (request, EXIT_ERR_ALREADY_JOINED, channelName);
		}
		
		else if (request.get_cmd () == "NAMES")
		{
			info = ":NAMES " + channelName + ": ",
				request.get_user ()->get_fd ();
			userIt = it->second.getMembers ().begin ();
			while (userIt != it->second.getMembers ().end ())
			{
				if (userIt != it->second.getMembers ().begin ())
					info += ",";
				info += (*userIt)->get_nickname ();	
				userIt++;
			}
			send_message (request, EXIT_INFO_ONLY, info);
		}
	}
}