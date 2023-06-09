/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_message.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pguranda <pguranda@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 02:37:04 by katchogl          #+#    #+#             */
/*   Updated: 2023/04/23 15:14:12 by pguranda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/////////////////////////////////////////////////
/// Server response messages config
/////////////////////////////////////////////////

void Server::send_message(Request req, t_res err)
{
    std::ostringstream stream;

	stream << static_cast<int>(err);
	switch (err)
	{
		case RES_ERR_UNKNOWNMODE:
			send_message (
				":" + std::string (SERVER_NAME)
				+ " " + stream.str ()
				+ " " + req.get_user ()->get_nickname ()
				+ " " + req.get_channel_name ()
				+ " :unknown mode"
				, req.get_user ()->get_fd ());
			break ;
		case RES_ERR_BADCHANNAME:
			send_message (
				":" + std::string (SERVER_NAME)
				+ " " + stream.str ()
				+ " " + req.get_user ()->get_nickname ()
				+ " " + req.get_channel_name ()
				+ " :bad channel name"
				, req.get_user ()->get_fd ());
			break ;
		case RES_RPL_LISTSTART:
			send_message (
				":" + std::string (SERVER_NAME)
				+ " " + stream.str ()
				+ " " + req.get_user ()->get_nickname ()
				+ " Channel: Users Name"
				, req.get_user ()->get_fd ());
			break ;
		case RES_RPL_LIST:
			send_message (
				":" + std::string (SERVER_NAME)
				+ " " + stream.str ()
				+ " " + req.get_user ()->get_nickname ()
				+ " " + req.get_info ()
				, req.get_user ()->get_fd ());
			break ;
		case RES_RPL_LISTEND:
			send_message (
				":" + std::string (SERVER_NAME)
				+ " " + stream.str ()
				+ " " + req.get_user ()->get_nickname ()
				+ " End of /LIST"
				, req.get_user ()->get_fd ());
			break ;
		case RES_ERR_NOSUCHNICK:
			send_message (
				":" + std::string (SERVER_NAME)
				+ " " + stream.str ()
				+ " " + req.get_user ()->get_nickname ()
				+ " " + req.get_info ()
				+ " :No such nick"
				, req.get_user ()->get_fd ());
			break ;
		/** With channel_name */
		case RES_ERR_CHANNOPRIVSNEEDED:
			send_message (
				":" + std::string (SERVER_NAME)
				+ " " + stream.str ()
				+ " " + req.get_user ()->get_nickname ()
				+ " " + req.get_channel_name ()
				+ " :You're not an operator"
				, req.get_user ()->get_fd ());
			break ;
		case RES_RPL_WHOREPLY:
			send_message (
				":" + std::string (SERVER_NAME)
				+ " " + stream.str ()
				+ " " + req.get_user ()->get_nickname ()
				+ " " + req.get_channel_name ()
				+ " " + req.get_info ()
				, req.get_user ()->get_fd ());
			break ;
		case RES_RPL_ENDOFWHO:
			send_message (
				":" + std::string (SERVER_NAME)
				+ " " + stream.str ()
				+ " " + req.get_user ()->get_nickname ()
				+ " " + req.get_channel_name ()
				+ " :End of /WHO list."
				, req.get_user ()->get_fd ());
			break ;
		case RES_RPL_TOPIC:
			send_message (
				":" + std::string (SERVER_NAME)
				+ " " + stream.str ()
				+ " " + req.get_user ()->get_nickname ()
				+ " " + req.get_channel_name ()
				+ " :" + req.get_info ()
				, req.get_user ()->get_fd ());
			break ;
		case RES_RPL_NOTOPIC:
			send_message (
				":" + std::string (SERVER_NAME)
				+ " " + stream.str ()
				+ " " + req.get_user ()->get_nickname ()
				+ " " + req.get_channel_name ()
				+ " :no topic is set"
				, req.get_user ()->get_fd ());
			break ;
		case RES_ERR_BADCHANNELKEY:
			stream.str ("");
			stream << static_cast<int>(RES_ERR_BADCHANNELKEY);
			send_message (
				":" + std::string (SERVER_NAME)
				+ " " + stream.str ()
				+ " " + req.get_user ()->get_nickname ()
				+ " " + req.get_channel_name ()
				+ " :password is incorrect"
				, req.get_user ()->get_fd ());
			break ;
		case RES_ERR_BANNEDFROMCHAN:
			send_message (
				":" + std::string (SERVER_NAME)
				+ " " + stream.str ()
				+ " " + req.get_user ()->get_nickname ()
				+ " " + req.get_channel_name ()
				+ " :permission denied. Have you provided the password?"
				, req.get_user ()->get_fd ());
			break ;
		case RES_ERR_NOSUCHCHANNEL:
			send_message (
				":" + std::string (SERVER_NAME)
				+ " " + stream.str ()
				+ " " + req.get_user ()->get_nickname ()
				+ " " + req.get_channel_name ()
				+ " :no such channel"
				, req.get_user ()->get_fd ());
			break ;
		case RES_ERR_NEEDMOREPARAMS:
			send_message (
				":" + std::string (SERVER_NAME)
				+ " " + stream.str ()
				+ " " + req.get_user ()->get_nickname ()
				+ " " + req.get_cmd ()
				+ " :not enough parameters"
				, req.get_user ()->get_fd ());
			break ;
		case RES_ERR_CHANNELISFULL :
			send_message (
				":" + std::string (SERVER_NAME)
				+ " " + stream.str ()
				+ " " + req.get_user ()->get_nickname ()
				+ " " + req.get_channel_name()
				+ " :Cannot join channel (+l)"
				, req.get_user ()->get_fd ());
			break ;
		case RES_ERR_NOTREGISTERED:
			send_message (
				":" + std::string (SERVER_NAME)
				+ " " + stream.str ()
				+ " " + req.get_user()->get_nickname()
				// + " " + req.get_channel_name ()
				+ " :You have not registered"
				, req.get_user ()->get_fd ());
			break ;
		/** Custom - Override response code or none */
		case RES_MODE:
			send_message (
				":" + std::string (SERVER_NAME)
				+ " MODE"
				+ " " + req.get_channel_name ()
				+ " " + req.get_info ()
				, req.get_user ()->get_fd ());
			break ;
		case RES_ERR_CHANNELALREADYJOINED:
			stream.str ("");
			stream << static_cast<int>(RES_ERR_BADCHANNAME);
			send_message (
				":" + std::string (SERVER_NAME)
				+ " " + stream.str ()
				+ " " + req.get_user ()->get_nickname ()
				+ " " + req.get_channel_name ()
				+ " :" + req.get_info ()
				+ " :already on channel"
				, req.get_user ()->get_fd ());
			break ;
		case RES_ERR_NOTONCHANNEL:
			send_message (
				":" + std::string (SERVER_NAME)
				+ " " + stream.str ()
				+ " " + req.get_user ()->get_nickname ()
				+ " " + req.get_channel_name ()
				+ " :You're not on that channel"
				, req.get_user ()->get_fd ());
			break ;
		case RES_ERR_USERNOTINCHANNEL:
			send_message (
				":" + std::string (SERVER_NAME)
				+ " " + stream.str ()
				+ " " + req.get_user ()->get_nickname ()
				+ " " + req.get_info ()
				+ " " + req.get_channel_name ()
				+ " :They aren't on the channel"
				, req.get_user ()->get_fd ());
			break ;
		case RES_ERR_NOTANOPERATOR:
			stream.str ("");
			stream << static_cast<int>(RES_ERR_CHANNOPRIVSNEEDED);
			send_message (
				":" + std::string (SERVER_NAME)
				+ " " + stream.str ()
				+ " " + req.get_user ()->get_nickname ()
				+ " " + req.get_channel_name ()
				+ " :" + req.get_info() + " is not an operator"
				, req.get_user ()->get_fd ());
			break ;
		case RES_ERR_ALREADYANOPERATOR:
			stream.str ("");
			stream << static_cast<int>(RES_ERR_USERSDONTMATCH);
			send_message (
				":" + std::string (SERVER_NAME)
				+ " " + stream.str ()
				+ " " + req.get_user ()->get_nickname ()
				+ " " + req.get_channel_name ()
				+ " :" + req.get_info () + " is already an operator"
				, req.get_user ()->get_fd ());
			break ;
		case RES_CHANNELLEFT:
			send_message (":" + req.get_user ()->get_nickname () + "!" 
				+ req.get_user ()->get_name () + "@" + SERVER_NAME + " PART "
				+ req.get_channel_name ()
				+ " :" + req.get_info (), req.get_user ()->get_fd ());
			break ;
		case RES_CHANNELJOINED:
			send_message (":" + req.get_user ()->get_nickname () + "!" 
				+ req.get_user ()->get_name () + "@" + SERVER_NAME + " JOIN "
				+ req.get_channel_name (), req.get_user ()->get_fd ());
			break ;
		default:
			send_message (
				":" + std::string (SERVER_NAME)
				+ " " + req.get_user ()->get_nickname ()
				+ " :an error occurred"
				, req.get_user ()->get_fd ());
	}
}
