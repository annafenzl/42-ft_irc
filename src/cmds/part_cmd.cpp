/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part_cmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katchogl <katchogl@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 12:24:20 by katchogl          #+#    #+#             */
/*   Updated: 2023/04/11 12:58:23 by katchogl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/Server.hpp"

void Server::part_command( Request request )
{
	std::string							info;
	std::list<User *>::const_iterator	userIt;

	if (request.get_params ().size () < 1
		&& request.get_user ()->get_channel () != NULL)
		request.get_params ().insert (request.get_params ().end (), 
				request.get_user ()->get_channel ()->getName ());
	else if (request.get_params ().size () < 1)
	{
		send_message (request, EXIT_ERR_NEEDMOREPARAMS, "");
		return ;
	}
	if (request.get_params ()[0] != request.get_user()->get_channel ()->getName ())
	{
		send_message (request, EXIT_ERR_NOTONCHANNEL, request.get_params ()[0]);
		return ;
	}
	userIt = std::find (request.get_user()->get_channel ()->getMembers ().begin (), 
		request.get_user()->get_channel ()->getMembers ().end (),
		request.get_user ());
	request.get_user()->get_channel ()->getMembers (0).erase (userIt);
	request.get_user()->set_channel (NULL);
	send_message (request, EXIT_LEFT_CHANNEL, request.get_params ()[0]);
}
