/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit_cmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pguranda <pguranda@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 16:05:10 by afenzl            #+#    #+#             */
/*   Updated: 2023/04/07 14:06:49 by pguranda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/Server.hpp"

// If a client connection is closed without the client issuing a QUIT command to the server,
// the server MUST distribute a QUIT message to other clients informing them of this,
// distributed in the same was an ordinary QUIT message.
// Servers MUST fill <reason> with a message reflecting the nature of the event which caused it to happen.

// since it has two possible usecases maybe overload this function, the other one with a passed reason, sent by the server

/*
	Command: QUIT
	Parameters: [ <Quit Message> ]

	A client session is terminated with a quit message.  The server
	acknowledges this by sending an ERROR message to the client.
*/

// could be send from client or server(in case of fatal error)
void Server::quit_command(Request request)
{
	User		*user = request.get_user();
	std::string	reason = (request.get_params().size() == 0) ? "" : request.get_params()[0];	

	// sends "Quit :<reason> to clients clients in the channel with him"
	

	// server acknowledges that by sending ERROR message
	send_message(SERVER_NAME " Error :" + reason, user->get_fd());

	// disconnect him
	for (unsigned int i = 0; i < _fd_count; ++i)
	{
		if (_user_poll[i].fd == user->get_fd())
		{
			std::cout << "USER " << user->get_nickname() << " hung up on " << user->get_fd() << std::endl;
			remove_from_poll(i);
		}
	}
	_user_map.erase(user->get_fd());
}
