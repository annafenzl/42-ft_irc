/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode_cmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katchogl <katchogl@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 00:45:11 by katchogl          #+#    #+#             */
/*   Updated: 2023/04/23 12:42:22 by katchogl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/Server.hpp"


void Server::mode_command( Request request)
{
	channelmap::iterator channelIt;
	std::string	updates;
	std::vector<std::string> params;
	std::string param;

	if (request.get_params ().size () < 1)
		return (send_message (request, RES_ERR_NEEDMOREPARAMS));
	
	request.set_channel_name (request.get_params ()[0]);

	if (!Channel::isValidChannelName (request.get_params ()[0]))
		return (send_message (request, RES_ERR_BADCHANNAME));
	
	channelIt = _channels.find (request.get_params ()[0]);

	if (channelIt == _channels.end ())
		return (send_message (request, RES_ERR_NOSUCHCHANNEL));

	params = request.get_params ();

	std::cout << "\033[0;31m params size:" << params.size () <<  "\033[0m" << std::endl;

	if (params.size () == 1)
	{
		request.set_info (channelIt->second.getModeAsString ());
		send_message (request, RES_MODE);
	}
	else
	{
		for (unsigned int i = 1; i < params.size(); ++i)
		{
			std::string::iterator it = params[i].begin();
			for ( ; it != params[i].end(); ++it)
			{
				char sign = *params[i].begin();
				param = "";
				if (it == params[i].begin() && (sign == '+' || sign == '-'))
				{
					// if (!updates.empty ())
					// 	updates += " ";
					// updates += sign;
					continue ;
				}
				if (!channelIt->second.isValidMode(*it))
				{
					std::cout << params[i] << std::endl;
					std::cout << "The value is: |" << *it << "|" <<std::endl;
					// std::cout << *it << " is not a valid mode " << std::endl;
					continue ;
				}
				else if (std::find (params[i].begin(), it, *it) != it)
				{
					std::cout << "\033[0;31mduplicate mode detected: " << *it << ", skipping\033[0m" << std::endl;
					continue ;
				}
				if (sign == '+' || sign == '-')
				{
					if (!channelIt->second.isOp (request.get_user()))
					{
						send_message (request, RES_ERR_CHANNOPRIVSNEEDED);
						continue ;
					}
					else if (Channel::isValidParamCase (*it, sign) && i >= params.size () - 1)
					{
						send_message (request, RES_ERR_NEEDMOREPARAMS);
						continue ;
					}
					else if (Channel::isValidParamCase (*it, sign))
						param = params[i + 1];
					if ( channelIt->second.execMode (*it, sign, param, *this, request))
					{
						channelIt->second.editMode (*it, sign);
						updates = sign;
						updates += *it;
						if (Channel::isValidParamCase (*it, sign))
							updates += " " + params[i + 1];
						request.set_info (updates);
						send_message (request, RES_MODE);
						broadcast (":" + std::string (SERVER_NAME)
							+ " MODE"
							+ " " + channelIt->second.getName ()
							+ " " + updates
							, request.get_user (), channelIt->second);
					}
					if (Channel::isValidParamCase (*it, sign))
					{
						i++;
						break;
					}
				}
			}
		}
	}
}