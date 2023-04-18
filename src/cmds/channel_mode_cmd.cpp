/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel_mode_cmd.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katchogl <katchogl@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 00:45:11 by katchogl          #+#    #+#             */
/*   Updated: 2023/04/18 07:50:47 by katchogl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/Server.hpp"

void Server::channel_mode_command( Request request )
{
	channelmap::iterator channelIt;
	size_t			i;
	size_t			j;
	std::string			nModes;
	std::string			updates;
	User				*user;
	User				*op;
	bool				isset;
	
	
	if (request.get_params ().size () < 1)
	{
		send_message (request, EXIT_ERR_NEEDMOREPARAMS, "");
		return ;
	}
	else if (!Channel::isValidChannelName (request.get_params ()[0]))
	{
		send_message (request, EXIT_ERR_INVALID_CHANNEL_NAME, request.get_params ()[0]);
		return ;
	}
	channelIt = _channels.find (request.get_params ()[0]);
	if (channelIt == _channels.end ())
	{
		send_message (request, EXIT_ERR_NOSUCHCHANNEL, request.get_params ()[0]);
		return ;
	}
	if (request.get_params ().size () > 1)
	{
		i = 0;
		while (++i < request.get_params ().size ())
		{
			if (request.get_params ()[i][0] != '+'
				&& request.get_params ()[i][0] != '-')
				continue ;
			j = 0;
			isset = false;
			while (request.get_params ()[i][++j])
			{
				if (request.get_params ()[i][j] == 'o' && request.get_params ()[i].length () == 2)
				{
					if (i == request.get_params ().size () - 1)
					{
						send_message (request, EXIT_ERR_NEEDMOREPARAMS, "");
						continue ;
					}
					user = channelIt->second.getMember (request.get_params ()[i + 1]);
					if (user == NULL)
					{
						send_message (":irc.example.com #channel : \x034Not on Channel!", request.get_user ()->get_fd ());
						continue ;
					}
					op = channelIt->second.getOp (user);
					if ((request.get_params ()[i][0] == '+' && op == NULL) ||
						(request.get_params ()[i][0] == '-' && op != NULL))
					{
						if (request.get_params ()[i][0] == '+' && op == NULL)
							channelIt->second.insertOp (op);
						else
							channelIt->second.removeOp (op);
						updates += " ";
						updates += request.get_params ()[i];
						updates += " ";
						updates += request.get_params ()[i + 1];
						isset = false;
					}
				}
				else if (request.get_params ()[i][j] != 'o' &&
					((request.get_params ()[i][0] == '+'
						&& !channelIt->second.hasMode (request.get_params ()[i][j]))
					|| (request.get_params ()[i][j] != 'o' && request.get_params ()[i][0] == '-'
						&& channelIt->second.hasMode (request.get_params ()[i][j]))))
				{
					std::cout << "\033[0;36m[LOG]current modes are: " << channelIt->second.getModes () << "\033[0m\n" << std::endl;
					if (request.get_params ()[i][0] == '+'
						&& !channelIt->second.hasMode (request.get_params ()[i][j]))
						channelIt->second.addMode (request.get_params ()[i][j]);
					else
						channelIt->second.removeMode (request.get_params ()[i][j]);
					if (!isset)
					{
						updates += " ";
						updates += request.get_params ()[i][0];
						isset = false;
					}
					std::cout << "\033[0;36m[LOG]mew modes are: " << channelIt->second.getModes () << "\033[0m\n" << std::endl;
					updates += request.get_params ()[i][j];
				}
			}
		}
	}
	if (updates.empty ())
		send_message (request, EXIT_MODE_STRING, request.get_params ()[0]
			+ " +" + channelIt->second.getModes ());
	else
		send_message (request, EXIT_MODE_STRING, request.get_params ()[0]
			+ updates);
}