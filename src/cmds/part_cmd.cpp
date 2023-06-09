/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part_cmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katchogl <katchogl@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 12:24:20 by katchogl          #+#    #+#             */
/*   Updated: 2023/04/23 12:01:25 by katchogl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Server.hpp"

/////////////////////////////////////////////////
/// Usage:
/// /PART ?<channel> ?<reason>
/////////////////////////////////////////////////

void Server::part_command( Request request )
{
	std::set<std::string>	channelNames;
	std::set<std::string>::iterator	channelNamesIt;

	std::string				dup;
	std::string				channelName;
	channelmap::iterator	channelIt;

	if (request.get_params ().size () < 1)
		return (send_message (request, RES_ERR_NEEDMOREPARAMS));
		
	channelNames = split_targets (request.get_params()[0], dup);
	if (channelNames.size () < 1)
		return (send_message (request, RES_ERR_NEEDMOREPARAMS));

	if (request.get_params ().size () > 1)
		request.set_info (request.get_params ()[1]);

	channelNamesIt = channelNames.begin ();
	while (channelNamesIt != channelNames.end ())
	{
		channelName = *channelNamesIt;
		request.set_channel_name (channelName);
	
		// check valid channel name
		if (!Channel::isValidChannelName (channelName))
		{
			send_message (request, RES_ERR_BADCHANNAME);
			channelNamesIt++;
			continue ;
		}

		// check whether channel exists
		channelIt = _channels.find (channelName);
		if (channelIt == _channels.end ())
		{
			send_message (request, RES_ERR_NOSUCHCHANNEL);
			channelNamesIt++;
			continue ;
		}

		// check whether user is on channel
		if (request.get_user ()->getChannels (0).find (channelName)
			== request.get_user ()->getChannels (0).end ())
		{
			request.set_info ("");
			send_message (request, RES_ERR_NOTONCHANNEL);
			channelNamesIt++;
			continue ;
		}
			
		// remove user from channel
		channelIt->second.remove (request.get_user ());
		// remove channel from user
		request.get_user ()->getChannels (0).erase
			(request.get_user ()->getChannels (0).find (channelName));
		// notify other members or remove channel if empty
		if (channelIt->second.getMembers ().size () != 0)
		{
			broadcast(":" + request.get_user ()->get_nickname() + "!" 
				+ request.get_user ()->get_nickname() + "@"  SERVER_NAME " PART " 
				+ channelIt->second.getName() + " :" + request.get_info ()
				, request.get_user(), channelIt->second);
		}
		else 
		{
			_channels.erase (channelIt);
			std::cout << "\033[0;32mSuccessfully deleted " 
				+ channelName + "\033[0m" << std::endl;
		}
		send_message (request, RES_CHANNELLEFT);
		channelNamesIt++;
	}
}
