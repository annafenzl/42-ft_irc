/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_cmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katchogl <katchogl@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 12:22:55 by katchogl          #+#    #+#             */
/*   Updated: 2023/04/20 19:32:24 by katchogl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Server.hpp"

void Server::list_command( Request request )
{
	std::set<std::string>				reqChannels;
	Server::channelmap::const_iterator	channelIt;
	std::string							dup;

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
