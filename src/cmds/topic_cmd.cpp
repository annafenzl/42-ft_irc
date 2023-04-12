/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic_cmd.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katchogl <katchogl@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 12:09:30 by katchogl          #+#    #+#             */
/*   Updated: 2023/04/11 12:58:11 by katchogl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/Server.hpp"

void Server::topic_command( Request request )
{
	std::string							info;
	channelmap::iterator				it;

	if (request.get_params ().size () < 1
		&& request.get_user ()->get_channel () != NULL)
		request.get_params ().insert (request.get_params ().end (), 
				request.get_user ()->get_channel ()->getName ());
	else if (request.get_params ().size () < 1)
	{
		send_message (request, EXIT_ERR_NEEDMOREPARAMS, "");
		return ;
	}
	it = _channels.find (request.get_params ()[0]);
	if (it == _channels.end ())
		return (send_message (request, EXIT_ERR_NOSUCHCHANNEL, request.get_params ()[0]));
	
	if (request.get_params ().size () == 1)
	{
		info = it->second.getTopic ();
		if (info == "*")
			info = "no topic";
		else
			info = "Current topic: " + info;
		send_message (request, EXIT_INFO_ONLY, info);
	}
	else
	{
		it->second.setTopic (request.get_params ()[1]);
		send_message (request, EXIT_INFO_ONLY, "Topic changed to: " 
			+ it->second.getTopic ());
	}
}
