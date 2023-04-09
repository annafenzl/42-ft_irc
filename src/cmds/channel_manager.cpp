/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel_manager.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katchogl <katchogl@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 13:07:12 by afenzl            #+#    #+#             */
/*   Updated: 2023/04/09 20:33:41 by katchogl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/exit.hpp"
# include "../../inc/Server.hpp"

t_exit Server::channel_manager( Request req )
{
	size_t							pos;
	std::string 					channelNames;
	std::string 					channelName;
	channelmap::iterator			it;
	std::list<User>::const_iterator	userIt;
	std::string						info;

	if ((req.get_cmd () == "JOIN"
		|| req.get_cmd () == "NAMES")
		&& req.get_params ().size () < 1)
			return (send_message (req, EXIT_ERR_NEEDMOREPARAMS, ""));
	if (req.get_cmd () == "JOIN"
		|| req.get_cmd () == "NAMES")
	{
		channelNames = req.get_params ()[0];
		while (true)
		{
			if (channelNames.empty ())
				break;
			
			pos = channelNames.find (",");
			channelName = channelNames.substr (0, pos);
			
			if (pos < channelNames.length ())
				channelNames = channelNames.substr (pos + 1);
			else
				channelNames = "";

			if (!Channel::isValidChannelName (channelName))
			{
				send_message (req, EXIT_ERR_INVALID_CHANNEL_NAME, channelName);
				continue ;
			}
			it = _channels.find (channelName);
			if (it == _channels.end () && req.get_cmd () == "JOIN")
			{
				_channels.insert (std::make_pair (channelName,
					Channel (channelName)));
				it = _channels.find (channelName);
			}
			else if (it == _channels.end () && req.get_cmd () == "NAMES")
			{
				send_message (req, EXIT_ERR_NOSUCHCHANNEL, channelName);
				continue ;
			}

			if (req.get_cmd () == "JOIN")
			{
				if (it->second.getMember (*req.get_user ()) != NULL)
				{
					send_message (req, EXIT_ERR_ALREADY_MEMBER, channelName);
					continue ;
				}
				it->second.join (*req.get_user ());
				send_message (req, EXIT_CHANNEL_JOINED, channelName);
			}
			
			else if (req.get_cmd () == "NAMES")
			{
				info = ":NAMES " + channelName + ": ",
					req.get_user ()->get_fd ();
				userIt = it->second.getMembers ().begin ();
				while (userIt != it->second.getMembers ().end ())
				{
					if (userIt != it->second.getMembers ().begin ())
						info += ",";
					info += userIt->get_nickname ();	
					userIt++;
				}
				send_message (req, EXIT_INFO_ONLY, info);
			}
		}
	}
	else if (req.get_cmd () == "TOPIC")
	{
		
	}
	return (EXIT_DEFAULT); // TODO: update
}
