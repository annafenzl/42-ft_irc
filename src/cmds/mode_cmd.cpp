/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode_cmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katchogl <katchogl@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 00:45:11 by katchogl          #+#    #+#             */
/*   Updated: 2023/04/20 11:16:37 by katchogl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/Server.hpp"

/////////////////////////////////////////////////
/// Usage:
/// /MODE <channel> ?[ (+/-)<modes> {args} ]
/////////////////////////////////////////////////

void Server::mode_command( Request request )
{
	channelmap::iterator channelIt;
	size_t			i;
	size_t			j;
	std::string			nModes;
	std::string			updates;
	User				*user;
	bool				isset;
	
	if (request.get_params ().size () < 1)
		return (send_message (request, RES_ERR_NEEDMOREPARAMS));
		
	request.set_channel_name (request.get_params ()[0]);
	
	if (!Channel::isValidChannelName (request.get_params ()[0]))
		return (send_message (request, RES_ERR_BADCHANNAME));
		
	channelIt = _channels.find (request.get_params ()[0]);
	
	if (channelIt == _channels.end ())
		return (send_message (request, RES_ERR_NOSUCHCHANNEL));
		
	if (request.get_params ().size () > 1)
	{
		i = 0;
		while (++i < request.get_params ().size ())
		{
			if (request.get_params ()[i][0] != '+' && request.get_params ()[i][0] != '-')
				continue ;
			else if (!channelIt->second.isOp (request.get_user ()))
				return (send_message (request, RES_ERR_CHANNOPRIVSNEEDED));

			j = 0;
			isset = false;
			while (request.get_params ()[i][++j])
			{

				if (request.get_params ()[i][j] == 'o' && request.get_params ()[i].length () == 2)
				{
					if (i == request.get_params ().size () - 1)
					{
						send_message (request, RES_ERR_NEEDMOREPARAMS);
						continue ;
					}
					user = channelIt->second.getMember (request.get_params ()[i + 1]);
					if (user == NULL)
					{
						send_message (":irc.example.com #channel : \x034Not on Channel!", request.get_user ()->get_fd ());
						continue ;
					}
					if ((request.get_params ()[i][0] == '+' && !channelIt->second.isOp (user))
						|| (request.get_params ()[i][0] == '-' && channelIt->second.isOp (user)))
					{
						if (request.get_params ()[i][0] == '+' && !channelIt->second.isOp (user))
							channelIt->second.insertOp (user);
						else
							channelIt->second.removeOp (user);
						updates += " " + request.get_params ()[i]  + " " + request.get_params ()[i + 1];
						isset = false;
					}
				}
				else if (request.get_params ()[i][j] != 'o' &&
					((request.get_params ()[i][0] == '+'
						&& !channelIt->second.hasMode (request.get_params ()[i][j]))
					|| (request.get_params ()[i][0] == '-'
						&& channelIt->second.hasMode (request.get_params ()[i][j]))))
				{
					std::cout << "\033[0;36m[LOG]current modes are: " <<
						channelIt->second.getModes () << "\033[0m\n" << std::endl;
					if (request.get_params ()[i][0] == '+'
						&& !channelIt->second.hasMode (request.get_params ()[i][j]))
						channelIt->second.addMode (request.get_params ()[i][j]);
					else
						channelIt->second.removeMode (request.get_params ()[i][j]);
					if (!isset)
					{
						updates += std::string(" ") + request.get_params ()[i][0];
						isset = true;
					}
					std::cout << "\033[0;36m[LOG]mew modes are: " << 
						channelIt->second.getModes () << "\033[0m\n" << std::endl;
					updates += request.get_params ()[i][j];
				}
			}
		}
	}
	if (updates.empty ())
		request.set_info ("+" + channelIt->second.getModes ());
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
