/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode_cmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afenzl <afenzl@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 00:45:11 by katchogl          #+#    #+#             */
/*   Updated: 2023/04/21 20:28:46 by afenzl           ###   ########.fr       */
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
	std::vector<std::string> params;

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

	params = request.get_params ();

	if (params.size() == 1)
		request.set_info (channelIt->second.getModeAsString ());
	
	else
	{
		for (unsigned int i = 1; i < params.size(); ++i)
		{
			for (std::string::const_iterator it = params[i].begin(); it != params[i].end(); ++it)
			{
				char sign = *params[i].begin();
				if (it == params[i].begin() && (sign == '+' || sign == '-'))
				{
					updates += sign;
					continue ;
				}
				
				if (!channelIt->second.isValidMode(*it))
				{ 
					std::cout << *it << " is not a valid mode " << std::endl;
					continue ;
				}
	
				if (sign  == '+' || sign == '-')
				{
					if (channelIt->second.execMode (*it, sign ,params, &i)) // TODO: exec, fetch next arg if needed and increment index
					{
						updates += *it;
						if (*it == 'o' || *it == 'k' || *it == 'l')
							updates += " " + params[i];
						channelIt->second.editMode (*it, sign);
					}
				}
				else
				{
					// get info and send response according to mode
				}
			}
			updates += " ";
		}
	}
	if (!updates.empty ())
		request.set_info (updates);
	else
		request.set_info (channelIt->second.getModeAsString ());
	send_message (request, RES_MODE);
}