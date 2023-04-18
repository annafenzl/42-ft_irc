/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part_cmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pguranda <pguranda@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 12:24:20 by katchogl          #+#    #+#             */
/*   Updated: 2023/04/18 09:37:20 by pguranda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/Server.hpp"

void Server::part_command( Request request )
{
	std::list<User *>::iterator							userIt;
	std::map<std::string, Channel *>::iterator				channelIt;
	channelmap::iterator				channelItServ;

	if (request.get_params ().size () < 1)
	{
		send_message (request, EXIT_ERR_NEEDMOREPARAMS, "");
		return ;
	}
	channelIt = request.get_user ()->getChannels (0).find (request.get_params ()[0]);
	std::cout << request.get_user ()->get_name () << " is initially in " 
		<< request.get_user ()->getChannels ().size () << "channels" << std::endl;
	// check if user on channel
	if (channelIt == request.get_user ()->getChannels (0).end ())
	{
		std::cout << "\033[0;31mnot on channel\033[0m" << std::endl;
		send_message (request, EXIT_ERR_NOTONCHANNEL, request.get_params ()[0]);
		return ;
	}
	// remove user from channel
	userIt = std::find (channelIt->second->getMembers (0).begin (), 
		channelIt->second->getMembers (0).end (),
		request.get_user ());
	channelIt->second->getMembers (0).erase (userIt);
	std::cout << channelIt->first << " now has: " 
		<< channelIt->second->getMembers (0).size () << " users" << std::endl;
	// remove channel from user
	request.get_user ()->getChannels (0).erase (channelIt);
	std::cout << request.get_user ()->get_name () << " is now in " 
		<< request.get_user ()->getChannels ().size () << "channels" << std::endl;
	// remove channel if empty
	channelItServ = _channels.find (request.get_params ()[0]);
	if (channelItServ != _channels.end ()
		&& channelItServ->second.getMembers ().size () == 0)
	{
		_channels.erase (channelItServ);
		std::cout << "\033[0;32mSuccessfully found and deleted " 
			+ request.get_params ()[0] + "\033[0m" << std::endl;
	}
	else if (channelItServ == _channels.end ())
		std::cout << "\033[0;31mFailed to find and delete channel\033[0m" << std::endl;
	std::cout << "there are now: " << _channels.size () << " channels" << std::endl;
	send_message (request, EXIT_LEFT_CHANNEL, request.get_params ()[0]);
}
