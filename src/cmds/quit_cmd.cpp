/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit_cmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pguranda <pguranda@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 16:05:10 by afenzl            #+#    #+#             */
/*   Updated: 2023/04/23 15:24:47 by pguranda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Server.hpp"

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
	std::string	reason = (request.get_params().size() == 0) ? "" : (":" + request.get_params()[0]);	

	// server acknowledges that by sending ERROR message
	send_message(SERVER_NAME " QUIT " + user->get_nickname() + " " + reason, user->get_fd());

	remove_user(user, reason);
}
