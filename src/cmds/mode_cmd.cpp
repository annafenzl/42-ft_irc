/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode_cmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afenzl <afenzl@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 00:45:11 by katchogl          #+#    #+#             */
/*   Updated: 2023/04/20 18:55:21 by afenzl           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/Server.hpp"

/////////////////////////////////////////////////
/// Usage:
/// /MODE <channel> ?[ (+/-)<modes> {args} ]
/////////////////////////////////////////////////

void Server::mode_command( Request request)
{
	channelmap::iterator channelIt;
	std::string	updates;

	if (request.get_params ().size () < 1)
		return (send_message (request, RES_ERR_NEEDMOREPARAMS));
	
	request.set_channel_name (request.get_params ()[0]);

	if (!Channel::isValidChannelName (request.get_params ()[0]))
		return (send_message (request, RES_ERR_BADCHANNAME));
	
	channelIt = _channels.find (request.get_params ()[0]);

	if (channelIt == _channels.end ())
		return (send_message (request, RES_ERR_NOSUCHCHANNEL));

	if (channelIt->second.isOp(request.get_user()) == false)
		return (send_message (request, RES_ERR_CHANNOPRIVSNEEDED));

	if (request.get_params().size() == 1)
		request.set_info (channelIt->second.getModeAsString('+'));
	
	else
	{
		std::string	update_modes = request.get_params()[1];
		char		sign = update_modes[0];
		
		if (sign == '+' || sign == '-')
		{
			for (std::string::const_iterator it = ++update_modes.begin(); it != update_modes.end(); ++it)
			{
				if (channelIt->second.isValidMode(*it) && channelIt->second.execMode(*it, request.get_params()))
					channelIt->second.editMode(*it, sign);
			}
		}
		request.set_info (channelIt->second.getModeAsString(sign));
	}
	send_message (request, RES_MODE);
}