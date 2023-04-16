/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_cmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katchogl <katchogl@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 12:22:55 by katchogl          #+#    #+#             */
/*   Updated: 2023/04/16 16:24:58 by katchogl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/Server.hpp"

void Server::list_command( Request request )
{
	std::string							info;
	Server::channelmap::const_iterator	channelIt;

	send_message (request, EXIT_RPL_LISTSTART, "");
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
			info = channelIt->first + " " + stream.str () + " " + channelIt->second.getTopic () ;
			channelIt++;
			send_message (request, EXIT_RPL_LIST, info);
		}
	}
	send_message (request, EXIT_RPL_LISTEND, "");
}
