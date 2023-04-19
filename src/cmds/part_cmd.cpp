/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part_cmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katchogl <katchogl@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 12:24:20 by katchogl          #+#    #+#             */
/*   Updated: 2023/04/19 05:32:54 by katchogl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Server.hpp"

/////////////////////////////////////////////////
/// Usage:
/// /PART ?<channel> ?<reason>
/////////////////////////////////////////////////

void Server::broadcast_part_message(User* user, Channel& channel)
{
	std::string join_message = ":" + user->get_nickname() + "!" + user->get_nickname() + "@"  SERVER_NAME " PART " + channel.getName();
	std::list<User*>::const_iterator userIt;
	
	for (userIt = channel.getMembers().begin(); userIt != channel.getMembers().end(); ++userIt)
		send_message(join_message, (*userIt)->get_fd());
}

void Server::part_command( Request request )
{
	channelmap::iterator								channelIt;

	if (request.get_params ().size () < 1)
		return (send_message (request, RES_ERR_NEEDMOREPARAMS));
		
	request.set_channel_name (request.get_params ()[0]);
	if (request.get_params ().size () > 1)
		request.set_info (request.get_params ()[1]);
	
	// check whether channel exist
	channelIt = _channels.find (request.get_params ()[0]);
	if (channelIt == _channels.end ())
		return (send_message (request, RES_ERR_NOSUCHCHANNEL));
	
	// check whether user on channel
	if (request.get_user ()->getChannels ().find (request.get_params ()[0])
		== request.get_user ()->getChannels ().end ())
		std::cout << "\033[0;31mnot on channel\033[0m" << std::endl;
		return (send_message (request, RES_ERR_NOTONCHANNEL));
		
	// remove user from channel
	channelIt->second.remove (request.get_user ());
	std::cout << channelIt->first << " now has: " 
		<< channelIt->second.getMembers (0).size () << " users" << std::endl;
	// remove channel from user
	request.get_user ()->getChannels (0).erase
		(request.get_user ()->getChannels (0).find (request.get_params ()[0]));
	std::cout << request.get_user ()->get_name () << " is now in " 
		<< request.get_user ()->getChannels ().size () << "channels" << std::endl;
	// Notify other members or remove channel if empty
	if (channelIt->second.getMembers ().size () != 0)
		broadcast_part_message(request.get_user(), channelIt->second);
	else 
	{
		_channels.erase (channelIt);
		std::cout << "\033[0;32mSuccessfully deleted " 
			+ request.get_params ()[0] + "\033[0m" << std::endl;
	}
	std::cout << "there are now: " << _channels.size () << " channels" << std::endl;
	send_message (request, RES_CHANNELLEFT);
}
