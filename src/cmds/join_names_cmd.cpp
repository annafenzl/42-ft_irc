/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_names_cmd.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pguranda <pguranda@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 12:21:43 by katchogl          #+#    #+#             */
/*   Updated: 2023/04/21 23:20:52 by pguranda         ###   ########.fr       */
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

void Server::broadcast (std::string message, User* user, Channel& channel)
{
	std::list<User*>::const_iterator userIt;
	
	for (userIt = channel.getMembers().begin(); userIt != channel.getMembers().end(); ++userIt)
		if ((*userIt) != user)
			send_message(message, (*userIt)->get_fd());
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
		return (send_message (request, RES_ERR_NEEDMOREPARAMS));

	channelNames = request.get_params ()[0];
	
	if (request.get_params ().size () > 1)
		passwords = request.get_params ()[1];
	else
		passwords = "*";
		
	while (!channelNames.empty ())
	{
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

		request.set_channel_name (channelName);
		
		if (!Channel::isValidChannelName (channelName))
		{
			send_message (request, RES_ERR_BADCHANNAME);
			continue ;
		}
		else if (!request.get_user ()->is_registered ())
		{
			send_message (request, RES_ERR_NOTREGISTERED_CHAN);
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
			send_message (request, RES_ERR_NOSUCHCHANNEL);
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
				send_message (request, RES_ERR_BANNEDFROMCHAN);
			else
				send_message (request, RES_ERR_BADCHANNELKEY);
			continue ;
		}
		//After JOIN, NAMES is ran automatically to show all the users in the channel in the client
		if (request.get_cmd () == "JOIN" && it->second.isMember (request.get_user ()))
			send_message (request, RES_ERR_CHANNELALREADYJOINED);
		else if (request.get_cmd () == "JOIN")
		{
			//Controlling for the channel limit, in case of the corresponding mode is set
			std::cout << "Channel limit: " << it->second.getLimit() << std::endl;
			std::cout << "Channel members: " << it->second.getMembers().size() << std::endl;
			if (it->second.hasMode('l') && it->second.getLimit() != -1 && static_cast<int>(it->second.getMembers().size()) >= it->second.getLimit())
			{
				send_message (SERVER_NAME " 471 " + request.get_user()->get_nickname() + it->second.getName() + " Cannot join channel (+l) - channel is full", request.get_user()->get_fd());
				continue;
			}
				
			send_message (request, RES_CHANNELJOINED);
			it->second.insert (request.get_user ());
			
			broadcast(":" + request.get_user ()->get_nickname() + "!" 
				+ request.get_user ()->get_nickname() + "@"  SERVER_NAME " JOIN " 
				+ it->second.getName(), request.get_user(), it->second);
				
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
			send_message (SERVER_NAME " 366 " + channelName + " :End of NAMES list", request.get_user()->get_fd());
		}
	}
}
