/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic_cmd.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katchogl <katchogl@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 12:09:30 by katchogl          #+#    #+#             */
/*   Updated: 2023/04/17 22:00:23 by katchogl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/Server.hpp"

void Server::topic_command( Request request )
{
	std::string							info;
	channelmap::iterator				it;

	if (request.get_params ().size () < 1)
	{
		send_message (request, EXIT_ERR_NEEDMOREPARAMS, "");
		return ;
	}
	it = _channels.find (request.get_params ()[0]);
	if (it == _channels.end ())
		return (send_message (request, EXIT_ERR_NOSUCHCHANNEL, request.get_params ()[0]));
	info = request.get_params ()[0] + " :";
	if (request.get_params ().size () == 1 
		&& (it->second.getTopic ().empty () || it->second.getTopic () == "*"))
	{
		send_message (request, EXIT_RPL_NOTOPIC, info + "no topic");
		return ;
	}
	if (request.get_params ().size () > 1)
		it->second.setTopic (request.get_params ()[1]);
	info += it->second.getTopic ();
	send_message (request, EXIT_RPL_TOPIC,  info);
	send_message (request, EXIT_TOPIC_STRING, info);
}
