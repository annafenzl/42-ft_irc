/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping_cmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katchogl <katchogl@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 11:36:00 by afenzl            #+#    #+#             */
/*   Updated: 2023/04/08 14:32:28 by katchogl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/Server.hpp"

/*
	Command: PING
	Parameters: <server1> [ <server2> ]
	The PING command is used to test the presence of an active client or
	server at the other end of the connection.
*/
void Server::ping_command(Request request)
{
	std::string response(SERVER_NAME);

	// ERR_NOORIGIN 
	if (request.get_params().size() == 0)
		response.append(" 409 " + request.get_user()->get_nickname() + " :No origin specified");

	else
		response.append(" PONG " SERVER_NAME);
	
	send_message(response, request.get_user()->get_fd());
}
