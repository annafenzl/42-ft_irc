/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part_cmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katchogl <katchogl@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 12:24:20 by katchogl          #+#    #+#             */
/*   Updated: 2023/04/12 21:33:04 by katchogl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/Server.hpp"

void Server::part_command( Request request )
{
	std::string							info;
	std::list<User *>::iterator			userIt;
	channelmap::iterator				channelIt;
	channelmap::iterator				channelItFound;

	if (request.get_params ().size () < 1
		&& request.get_user ()->get_channel () != NULL)
		request.get_params ().insert (request.get_params ().end (), 
				request.get_user ()->get_channel ()->getName ());
	else if (request.get_params ().size () < 1)
	{
		send_message (request, EXIT_ERR_NEEDMOREPARAMS, "");
		return ;
	}
	// check if user is on channel to part from
	if (request.get_params ()[0] != request.get_user()->get_channel ()->getName ())
	{
		std::cout << "\033[0;31mUser is not in channel " + request.get_params ()[0]
			+ " but in channel " + request.get_user()->get_channel ()->getName () 
			+ "\033[0m" << std::endl;
		send_message (request, EXIT_ERR_NOTONCHANNEL, request.get_params ()[0]
			+ " vs " + request.get_user()->get_channel ()->getName ());
		return ;
	}
	// remove user from channel
	userIt = std::find (request.get_user()->get_channel ()->getMembers (0).begin (), 
		request.get_user()->get_channel ()->getMembers (0).end (),
		request.get_user ());
	request.get_user()->get_channel ()->getMembers (0).erase (userIt);
	// remove channel if empty
	if (request.get_user()->get_channel ()->getMembers (0).size () == 0)
	{
		channelIt = _channels.find (request.get_user()->get_channel ()->getName ());
		if (channelIt != _channels.end ())
		{
			info = request.get_user()->get_channel ()->getName ();
			_channels.erase (channelIt);
			std::cout << "\033[0;32mSuccessfully found and deleted " 
				+ info + "\033[0m" << std::endl;
		}
		else
			std::cout << "\033[0;31mFailed to find and delete channel\033[0m" << std::endl;
	}
	info = "";
	// remove channel from user
	request.get_user()->set_channel (NULL);
	// find latest channel in which user is, else stays NULL
	channelIt = _channels.begin ();
	channelItFound = _channels.end ();
	while (channelIt != _channels.end ())
	{
		std::cout << "[DEBUG] searching in channel: " + channelIt->first << std::endl;
		userIt = std::find (channelIt->second.getMembers (0).begin (),
			channelIt->second.getMembers (0).end (), request.get_user());
		if (userIt != channelIt->second.getMembers (0).end ())
			channelItFound = channelIt;
		channelIt++;
	}
	if (channelItFound != _channels.end ())
	{
		std::cout << "\033[0;36muser is now in channel: " + channelItFound->first + "\033[0m" << std::endl;
		request.get_user ()->set_channel (&channelItFound->second);
	}
	else
		std::cout << "\033[0;36muser is not in any channel\033[0m" << std::endl;
	send_message (request, EXIT_LEFT_CHANNEL, request.get_params ()[0]);
}
