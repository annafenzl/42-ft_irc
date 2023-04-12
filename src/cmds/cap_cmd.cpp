/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cap_cmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pguranda <pguranda@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 11:35:55 by afenzl            #+#    #+#             */
/*   Updated: 2023/04/12 14:27:18 by pguranda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/Server.hpp"

/*
	Command: CAP
	Parameters: LS / REQ
	Upon connecting to the IRC server,
	CAP command is a way to negotiate capabilities (protocol extensions) with the server;
	using the subcommands below to list and request capabilities.
*/
void Server::cap_command(Request request)
{
	std::string response;

	// CAP LS [version] to discover the available capabilities on the server.
	if (request.get_params()[0] == "LS")
	{
		response = (SERVER_NAME + " CAP * LS :End of CAP LS negotiation");
		send_message(response, request.get_user()->get_fd());
	}
	
	// CAP REQ to blindly request a particular set of capabilities.
}

