/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel_manager.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katchogl <katchogl@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 13:07:12 by afenzl            #+#    #+#             */
/*   Updated: 2023/04/10 01:55:19 by katchogl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/exit.hpp"
# include "../../inc/Server.hpp"

t_exit Server::channel_manager( Request req )
{
	size_t								pos;
	std::string 						channelNames;
	std::string 						channelName;
	channelmap::iterator				it;
	std::list<User *>::const_iterator	userIt;
	Server::channelmap::const_iterator	channelIt;
	std::string							info;

	if (req.get_cmd () == "JOIN")
	{
		if (req.get_params ().size () < 1)
			return (send_message (req, EXIT_ERR_NEEDMOREPARAMS, ""));
		else if (req.get_user ()->get_channel () != NULL)
			return (send_message (req, EXIT_ERR_ALREADY_JOINED,
				req.get_user ()->get_channel ()->getName ()));
	}
	else if ((req.get_cmd () == "NAMES" || req.get_cmd () == "TOPIC"
		||  req.get_cmd () == "PART")
		&& req.get_params ().size () < 1)
	{
		if (req.get_user ()->get_channel () != NULL)
			req.get_params ().insert (req.get_params ().end (), 
				req.get_user ()->get_channel ()->getName ());
		else if (req.get_cmd () != "PART")
			return (send_message (req, EXIT_ERR_NEEDMOREPARAMS, ""));
		else
			return (EXIT_DEFAULT);
	}
	
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
				req.get_user ()->set_channel (&it->second);
				it->second.join (req.get_user ());
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
					info += (*userIt)->get_nickname ();	
					userIt++;
				}
				send_message (req, EXIT_INFO_ONLY, info);
			}
		}
	}
	
	else if (req.get_cmd () == "LIST")
	{
		channelIt = _channels.begin ();
		while (channelIt != _channels.end ())
		{
		    std::ostringstream					stream;

			stream << channelIt->second.getMembers ().size ();
			info = channelIt->first + " : " + stream.str () + " member";
			if (channelIt->second.getMembers ().size () != 1)
				info += "s";
			channelIt++;
			send_message (req, EXIT_INFO_ONLY, info);
		}
	}
	
	else if (req.get_cmd () == "TOPIC")
	{
		it = _channels.find (req.get_params ()[0]);
		
		if (it == _channels.end ())
			return (send_message (req, EXIT_ERR_NOSUCHCHANNEL, req.get_params ()[0]));
		
		if (req.get_params ().size () == 1)
		{
			info = it->second.getTopic ();
			if (info == "*")
				info = "no topic";
			else
				info = "Current topic: " + info;
			return (send_message (req, EXIT_INFO_ONLY, info));
		}
		else
		{
			it->second.setTopic (req.get_params ()[1]);
			return (send_message (req, EXIT_INFO_ONLY, "Topic changed to: " 
				+ it->second.getTopic ()));
		}
	}

	else if (req.get_cmd () == "PART")
	{
		if (req.get_params ().size () < 1 || req.get_user()->get_channel () == NULL
			|| req.get_params ()[0] != req.get_user()->get_channel ()->getName ())
			return (EXIT_ERR_NOTONCHANNEL);
		userIt = std::find (req.get_user()->get_channel ()->getMembers ().begin (), 
			req.get_user()->get_channel ()->getMembers ().end (),
			req.get_user ());
		req.get_user()->get_channel ()->getMembers (0).erase (userIt);
		req.get_user()->set_channel (NULL);

		return (send_message (req, EXIT_LEFT_CHANNEL, req.get_params ()[0]));
	}

	return (EXIT_DEFAULT);
}
