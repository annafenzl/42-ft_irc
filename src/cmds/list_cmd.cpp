/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_cmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pguranda <pguranda@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 12:22:55 by katchogl          #+#    #+#             */
/*   Updated: 2023/04/19 14:30:37 by pguranda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Server.hpp"

void Server::list_command( Request request )
{
	Server::channelmap::const_iterator	channelIt;
	User *user = request.get_user ();

	if (!user->is_registered())
		send_message( SERVER_NAME " 462 " + user->get_nickname() + " :Unauthorized command (not yet registered)", user->get_fd());
	if (request.get_params ().size () )
		send_message (request, RES_RPL_LISTSTART);
	channelIt = _channels.begin ();
	while (channelIt != _channels.end ())
	{
		std::ostringstream					stream;
		if (request.get_params ().size () < 1 
			|| std::find (request.get_params ().begin (),
				request.get_params ().end (), channelIt->first)
				!= request.get_params ().end ())
		{
			stream << channelIt->second.getMembers ().size ();
			request.set_info (channelIt->first + " " + stream.str () 
				+ " :" + channelIt->second.getTopic ());
			channelIt++;
			send_message (request, RES_RPL_LIST);
		}
	}
	send_message (request, RES_RPL_LISTEND);
}
