/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic_cmd.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katchogl <katchogl@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 12:09:30 by katchogl          #+#    #+#             */
/*   Updated: 2023/04/19 03:02:30 by katchogl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Server.hpp"

void Server::topic_command( Request request )
{
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
	{
		send_message (request, RES_RPL_NOTOPIC);
		return ;
	}
	if (request.get_params ().size () > 1
		&& (!it->second.hasMode ('t') || it->second.isOp (request.get_user ())))
		it->second.setTopic (request.get_params ()[1]);
	else if (request.get_params ().size () > 1)
	{
		send_message (request, RES_ERR_CHANNOPRIVSNEEDED);
		return ;
	}
	request.set_info (it->second.getTopic ());
	send_message (request, RES_RPL_TOPIC);
}
