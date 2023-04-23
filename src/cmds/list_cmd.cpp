/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_cmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afenzl <afenzl@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 12:22:55 by katchogl          #+#    #+#             */
/*   Updated: 2023/04/23 13:28:45 by afenzl           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Server.hpp"

void Server::list_command( Request request )
{
	std::set<std::string>				reqChannels;
	Server::channelmap::const_iterator	channelIt;
	User *user = request.get_user ();
	std::string							dup;

	if (!user->is_registered())
		return(send_message( request, RES_ERR_NOTREGISTERED));

	if (request.get_params ().size () )
		send_message (request, RES_RPL_LISTSTART);
	channelIt = _channels.begin ();

	if (request.get_params ().size () > 0)
		reqChannels = split_targets (request.get_params ()[0], dup);
		
	while (channelIt != _channels.end ())
	{
		if (request.get_params ().size () == 0
			|| reqChannels.find (channelIt->first) != reqChannels.end ())
		{
			std::ostringstream	stream;
			stream << channelIt->second.getMembers ().size ();
			request.set_info (channelIt->first + " " + stream.str () 
				+ " :" + channelIt->second.getTopic ());
			send_message (request, RES_RPL_LIST);
		}
		channelIt++;
	}
	send_message (request, RES_RPL_LISTEND);
}
