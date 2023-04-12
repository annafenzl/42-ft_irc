/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_cmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katchogl <katchogl@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 12:22:55 by katchogl          #+#    #+#             */
/*   Updated: 2023/04/11 18:39:36 by katchogl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/Server.hpp"

void Server::list_command( Request request )
{
	std::string							info;
	Server::channelmap::const_iterator	channelIt;

	channelIt = _channels.begin ();
	while (channelIt != _channels.end ())
	{
		std::ostringstream					stream;

		stream << channelIt->second.getMembers ().size ();
		info = channelIt->first + " : " + stream.str () + " member";
		if (channelIt->second.getMembers ().size () != 1)
			info += "s";
		channelIt++;
		send_message (request, EXIT_INFO_ONLY, info);
	}
}
