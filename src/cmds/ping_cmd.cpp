/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping_cmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katchogl <katchogl@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 11:36:00 by afenzl            #+#    #+#             */

/*   Updated: 2023/04/06 11:19:23 by afenzl           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include Server.hpp

/*
	Command: PING
	Parameters: <token>
	The PING command is used to test the presence of an active client or
	server at the other end of the connection.
*/
void Server::ping_command(Request request)
{
	std::string response(SERVER_NAME);

	// ERR_NEEDMOREPARAMS
	if (request.get_params().size() == 0)
		response.append(" 461 " + request.get_user()->get_nickname() + " :Not enough parameters.");

	// ERR_NOORIGIN 
	else if (request.get_params()[0].empty())
		response.append(" 409 " + request.get_user()->get_nickname() + " :No origin specified");

	else
		response.append(" PONG " + request.get_params()[0]);
	
	send_message(response, request.get_user()->get_fd());
}
