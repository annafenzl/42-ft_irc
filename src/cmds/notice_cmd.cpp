/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   notice_cmd.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annafenzl <annafenzl@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 15:22:07 by annafenzl         #+#    #+#             */
/*   Updated: 2023/04/06 15:31:08 by annafenzl        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/Server.hpp"

/*
	Command: NOTICE
	Parameters: <target>{,<target>} <text to be sent>

	automatic replies must never be sent in response to a NOTICE message.
	This rule also applies to servers
	– they must not send any error back to the client on receipt of a NOTICE command.
*/
void Server::notice_command(Request request)
{
	// for now just same behavior as privmsg
	privmsg_command(request);
}
