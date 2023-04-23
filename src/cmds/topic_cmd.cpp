/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic_cmd.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pguranda <pguranda@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 12:09:30 by katchogl          #+#    #+#             */
/*   Updated: 2023/04/22 15:28:44 by pguranda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Server.hpp"

void Server::topic_command( Request request )
{
	std::ostringstream				stream;
	channelmap::iterator				it;

	if (request.get_params ().size () < 1)
	{
		send_message (request, RES_ERR_NEEDMOREPARAMS);
		return ;
	}
	request.set_channel_name (request.get_params ()[0]);
	it = _channels.find (request.get_params ()[0]);
	if (it == _channels.end ())
		return (send_message (request, RES_ERR_NOSUCHCHANNEL));
	if (request.get_params ().size () == 1 
		&& (it->second.getTopic ().empty () || it->second.getTopic () == "*"))
		return (send_message (request, RES_RPL_NOTOPIC));
	if (request.get_params ().size () > 1
		&& (!it->second.hasMode ('t') || it->second.isOp (request.get_user ())))
		it->second.setTopic (request.get_params ()[1]);
	else if (request.get_params ().size () > 1)
		return (send_message (request, RES_ERR_CHANNOPRIVSNEEDED));
	request.set_info (it->second.getTopic ());
	send_message (request, RES_RPL_TOPIC);

	stream << static_cast<int>(RES_RPL_TOPIC);
	broadcast (":" + std::string (SERVER_NAME)
				+ " " + stream.str ()
				+ " " + request.get_user ()->get_nickname ()
				+ " " + request.get_channel_name ()
				+ " :" + request.get_info (), request.get_user (), it->second);
}
