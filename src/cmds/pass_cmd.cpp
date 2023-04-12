/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass_cmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katchogl <katchogl@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 11:49:01 by afenzl            #+#    #+#             */

/*   Updated: 2023/04/06 10:55:40 by afenzl           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"

/*
	Command: PASS
	Parameters: <password>
	The PASS command is used to set a 'connection password'.  The
	optional password can and MUST be set before any attempt to register
	the connection is made.  Currently this requires that user send a
	PASS command before sending the NICK/USER combination.
*/
void Server::pass_command(Request request)
{
	std::string response(SERVER_NAME);
	User		*user = request.get_user();

	// ERR_ALREADYREGISTRED
	if (user->is_registered())
		response.append(SERVER_NAME " 462 " + user->get_nickname() + " :Unauthorized command (already registered)");

	// ERR_NEEDMOREPARAMS
	else if (request.get_params().size() != 1)
		response.append(SERVER_NAME " 461 " + user->get_nickname() + " PASS :Not the right amount of parameters");

	// ERR_PASSWDMISMATCH 
	else if (request.get_params()[0] != _password)
	{
		response.append(SERVER_NAME " 464 " + user->get_nickname() + " :Password incorrect");
		user->set_pass_provided(false);
	}

	else
	{
		user->set_pass_provided(true);
		return;
	}
	send_message(response, user->get_fd());
}
