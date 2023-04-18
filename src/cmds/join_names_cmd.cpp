/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_names_cmd.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katchogl <katchogl@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 12:21:43 by katchogl          #+#    #+#             */
/*   Updated: 2023/04/18 23:07:51 by katchogl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Server.hpp"

/************************************************
 *	! /JOIN /NAMES handler !
 *
 *	usage:	/JOIN #channel1,#channel2
 *			/NAMES #channel1,#channel2
 ***********************************************/

void Server::send_names_list(Request &request, Channel &channel)
{
	std::string info;
	std::list<User *>::const_iterator userIt;

	userIt = channel.getMembers().begin();
	while (userIt != channel.getMembers().end())
	{
		if (userIt != channel.getMembers().begin())
			info += ' ';
		info += (*userIt)->get_nickname();
		userIt++;
	}
	send_message(SERVER_NAME " 353 " + request.get_user()->get_nickname() + " = " + channel.getName() + " :" + info, request.get_user()->get_fd());
	send_message(SERVER_NAME " 366 " + request.get_user()->get_nickname() + " " + channel.getName() + " :End of NAMES list", request.get_user()->get_fd());
}	


void Server::join_names_command( Request request )
{
	size_t								pos;
	std::string 						channelNames;
	std::string 						channelName;
	std::string							info;
	channelmap::iterator				it;
	std::list<User *>::const_iterator	userIt;

	std::string 						passwords;
	std::string 						password;
	size_t								pos2;

	if (request.get_params ().size () < 1)
	{
		send_message (request, EXIT_ERR_NEEDMOREPARAMS, "");
		return ;
	}
	
	channelNames = request.get_params ()[0];
	
	if (request.get_params ().size () > 1)
		passwords = request.get_params ()[1];
	else
		passwords = "*";
		
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
	
		pos2 = passwords.find (",");
		password = passwords.substr (0, pos2);
		
		if (pos2 < passwords.length ())
			passwords = passwords.substr (pos2 + 1);
		else
			passwords = "*";
		
		if (!Channel::isValidChannelName (channelName))
		{
			send_message (request, EXIT_ERR_INVALID_CHANNEL_NAME, channelName);
			continue ;
		}
		it = _channels.find (channelName);

		if (it == _channels.end () && request.get_cmd () == "JOIN")
		{
			_channels.insert (std::make_pair (channelName,
				Channel (channelName, password)));
			it = _channels.find (channelName);
		}
		else if (it == _channels.end () && request.get_cmd () == "NAMES")
		{
			send_message (request, EXIT_ERR_NOSUCHCHANNEL, channelName);
			continue ;
		}
		else if (it != _channels.end () && request.get_cmd () == "JOIN"
			&& it->second.getPassword () != "*"
			&& (password == "*" || password != it->second.getPassword ()))
		{
			std::cout << "\033[0;31m[ERROR] Failed to join channel: " 
				+ it->second.getName ()
				+ " provided: '" + password 
				+ "', channel password: '" + it->second.getPassword ()
				+ "'\033[0m" << std::endl;
			if (password == "*")
				send_message (request, EXIT_ERR_BANNEDFROMCHAN, channelName);
			else
				send_message (request, EXIT_ERR_BADCHANNELKEY, channelName);
			continue ;
		}
		//After JOIN, NAMES is ran automatically to show all the users in the channel in the client
		if (request.get_cmd () == "JOIN")
		{
			if (it->second.getMember (request.get_user ()) == NULL)
				send_message (request, EXIT_CHANNEL_JOINED, channelName);
			else
				send_message (request, EXIT_ERR_ALREADY_JOINED, channelName);
			it->second.insert (request.get_user ());
			request.get_user ()->getChannels (0).insert (std::make_pair (it->first, &it->second));
			send_names_list(request, it->second );
		}
		
		else if (request.get_cmd () == "NAMES")
		{
			userIt = it->second.getMembers ().begin ();
			while (userIt != it->second.getMembers ().end ())
			{
				if (userIt != it->second.getMembers ().begin ())
					info += ' ';
				info += (*userIt)->get_nickname ();	
				userIt++;
			}
			send_message (SERVER_NAME " 353 " + request.get_user ()->get_nickname () + " = " + channelName + " :" + info, request.get_user ()->get_fd ());
			send_message(SERVER_NAME " 366 " + channelName + " :End of NAMES list", request.get_user()->get_fd());
		}
	}
}