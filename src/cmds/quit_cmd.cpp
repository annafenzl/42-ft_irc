/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit_cmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afenzl <afenzl@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 16:05:10 by afenzl            #+#    #+#             */
/*   Updated: 2023/03/22 10:33:16 by afenzl           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/Server.hpp"

/*
	Command: QUIT
	Parameters: [ <Quit Message> ]

	A client session is terminated with a quit message.  The server
	acknowledges this by sending an ERROR message to the client.
*/
void Server::quit_command(Request request)
{
	std::string response;
	User		*user = request.get_user();

	send_message(response, user->get_fd());
}
