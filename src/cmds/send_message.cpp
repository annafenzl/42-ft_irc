/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_message.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katchogl <katchogl@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 14:24:24 by katchogl          #+#    #+#             */
/*   Updated: 2023/04/08 19:39:25 by katchogl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/Server.hpp"

t_errco Server::send_message(Request req, t_errco err)
{
	std::string mes;
    std::ostringstream stream;

	stream << static_cast<int>(err);
	mes.append (SERVER_NAME).append (" " + stream.str ()
		+ " " + req.get_user ()->get_nickname () + " :");
		
	if (err == ERR_NEEDMOREPARAMS)
		mes.append ("need more parameters");
	else if (err == ERR_TOOMANYCHANNELS)
		mes.append ("Too many channels");
	else
		mes.append ("error code not found");
	
	send_message (mes, req.get_user ()->get_fd ());
	return (err);
}
