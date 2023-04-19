/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_cmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katchogl <katchogl@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 12:22:55 by katchogl          #+#    #+#             */
/*   Updated: 2023/04/19 02:06:57 by katchogl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Server.hpp"

void Server::list_command( Request request )
{
	Server::channelmap::const_iterator	channelIt;

	send_message2 (request, RES_RPL_LISTSTART);
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
			request.set_info (channelIt->first + " " + stream.str () + " " + channelIt->second.getTopic ());
			channelIt++;
			send_message2 (request, RES_RPL_LIST);
		}
	}
	send_message2 (request, RES_RPL_LISTEND);
}