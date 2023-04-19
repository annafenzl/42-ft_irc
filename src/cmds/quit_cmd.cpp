/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit_cmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afenzl <afenzl@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 16:05:10 by afenzl            #+#    #+#             */
/*   Updated: 2023/04/19 14:22:20 by afenzl           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Server.hpp"

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

	// server acknowledges that by sending ERROR message
	// send_message(SERVER_NAME " QUIT: " + user->get_nickname() + " " + reason, user->get_fd());

	remove_user(user, reason);
}
