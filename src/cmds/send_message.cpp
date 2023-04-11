/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_message.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pguranda <pguranda@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 14:24:24 by katchogl          #+#    #+#             */
/*   Updated: 2023/04/11 14:42:22 by pguranda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/exit.hpp"
# include "../../inc/Server.hpp"

t_exit Server::send_message(Request req, t_exit err, std::string info)
{
	std::string mes;
    std::ostringstream stream;

	stream << static_cast<int>(err);
	mes.append (SERVER_NAME).append (" " + stream.str ()
		+ " " + req.get_user ()->get_nickname () + " :");

	switch (err)
	{
		case EXIT_CHANNEL_JOINED:
			mes.append ("joined channel: " + info + ": success");
			break;
		case EXIT_RPL_ENDOFNAMES:
			mes.append ("end of /NAME list");
			break;
		case EXIT_RPL_NAMREPLY:
			mes.append ("channel: " + info);
			break;
	
		case EXIT_ERR_NEEDMOREPARAMS:
			mes.append ("need more parameters");
			break;
		case EXIT_ERR_TOOMANYCHANNELS:
			mes.append ("Too many channels");
			break;
		case EXIT_ERR_ALREADY_JOINED:
			mes.append ("already joined channel: " + info);
			break;
		case EXIT_ERR_INVALID_CHANNEL_NAME:
			mes.append ("invalid channel name");
			break;
		case EXIT_ERR_NOSUCHCHANNEL:
			mes.append ("no such channel: " + info);
			break;
		case EXIT_INFO_ONLY:
			mes.append (info);
			break;
		case EXIT_LEFT_CHANNEL:
			mes.append ("you left: " + info);
			break;
		default:
			mes.append ("error code not found");
			break;
	}
	
	send_message (mes, req.get_user ()->get_fd ());
	return (err);
}

