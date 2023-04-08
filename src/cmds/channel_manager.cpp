/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel_manager.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katchogl <katchogl@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 13:07:12 by afenzl            #+#    #+#             */
/*   Updated: 2023/04/08 20:12:41 by katchogl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/errco.hpp"
# include "../../inc/Server.hpp"

t_errco Server::channel_manager( Request request )
{
	channelmap::iterator it;
	
	if (request.get_cmd () == "JOIN")
	{
		if (request.get_params ().size () < 1)
			return (send_message (request, ERR_NEEDMOREPARAMS));
		it = _channels.find (request.get_params ()[0]);
		if (it == _channels.end ())
		{
			_channels.insert (std::make_pair (request.get_params ()[0],
				Channel (request.get_params ()[0], User (*request.get_user ()))));
		}
		else
		{
			std::cout << "channel found" << std::endl;
		}
	}
	else if (request.get_cmd () == "TOPIC")
	{
		std::cout << "TOPIC" << std::endl;
	}
	return (ERR_NONE);
}
