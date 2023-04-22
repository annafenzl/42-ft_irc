/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode_cmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katchogl <katchogl@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 00:45:11 by katchogl          #+#    #+#             */
/*   Updated: 2023/04/22 02:58:28 by katchogl         ###   ########.fr       */
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

	if (params.size() == 1)
		request.set_info (channelIt->second.getModeAsString ());
	else
	{
		for (unsigned int i = 1; i < params.size(); ++i)
		{
			for (std::string::iterator it = params[i].begin(); it != params[i].end(); ++it)
			{
				char sign = *params[i].begin();
				param = "";
				if (it == params[i].begin() && (sign == '+' || sign == '-'))
				{
					if (!updates.empty ())
						updates += " ";
					updates += sign;
					continue ;
				}
				if (!channelIt->second.isValidMode(*it))
				{
					std::cout << *it << " is not a valid mode " << std::endl;
					continue ;
				}
				else if (std::find (params[i].begin(), it, *it) != it)
				{
					std::cout << "\033[0;31mduplicate mode detected: " << *it << ", skipping\033[0m" << std::endl;
					continue ;
				}
				if (sign == '+' || sign == '-')
				{
					if (!channelIt->second.isOp (request.get_user())) // check if user is op
					{
						send_message (request, RES_ERR_CHANNOPRIVSNEEDED);
						continue ;
					}
					else if (Channel::isValidArgMode (*it) && i >= params.size () - 1) // check if not last if is mode with arg
					{
						send_message (request, RES_ERR_NEEDMOREPARAMS);
						continue ;
					}
					if (Channel::isValidArgMode (*it)) // set and check arg if is mode with arg
					{
						param = params[i + 1];
						if (param[0] == '+' || param[0] == '-')
						{
							send_message (request, RES_ERR_NEEDMOREPARAMS);
							continue ;
						}
						i++;
					}
					if (channelIt->second.execMode (*it, sign, param, *this, request)) // execute. if successful, update channel modes
					{
						if (Channel::isValidArgMode (*it))
							channelIt->second.editMode (*it, sign);
						updates += *it;
						if (Channel::isValidArgMode (*it))
							updates += " " + param;
					}
				}
			}
			updates += " ";
		}
	}
	std::cout << "updates are: |" << updates << "|" << std::endl;
	if (updates.empty () || updates.find_first_not_of(' ') == std::string::npos)
		request.set_info (channelIt->second.getModeAsString ());
	else
	{
		request.set_info (updates);
		broadcast (":" + std::string (SERVER_NAME)
			+ " MODE"
			+ " " + channelIt->second.getName ()
			+ " " + updates
			, request.get_user (), channelIt->second);
	}
	send_message (request, RES_MODE);
}