/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_cmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afenzl <afenzl@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 13:07:12 by afenzl            #+#    #+#             */
/*   Updated: 2023/03/22 10:30:03 by afenzl           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/Server.hpp"

//  Once a user has joined a channel, he receives information about
//    all commands his server receives affecting the channel.  This
//    includes JOIN, MODE, KICK, PART, QUIT and of course PRIVMSG/NOTICE.
//    This allows channel members to keep track of the other channel
//    members, as well as channel modes.

//    If a JOIN is successful, the user receives a JOIN message as
//    confirmation and is then sent the channel's topic (using RPL_TOPIC) and
//    the list of users who are on the channel (using RPL_NAMREPLY), which
//    MUST include the user joining.

// REMEMBER!!!!
//    Note that this message accepts a special argument ("0"), which is
//    a special request to leave all channels the user is currently a member
//    of. The server will process this message as if the user had sent
//    a PART command (See Section 3.2.2) for each channel he is a member
//    of.

/*
	Command: JOIN
	Parameters: ( <channel> *( "," <channel> ) [ <key> *( "," <key> ) ] )/
	The JOIN command is used by a user to request to start listening to
	the specific channel.
*/
void Server::join_command(Request request)
{
	std::string response;
	User		*user;
	
	// ERR_NEEDMOREPARAMS
	if (request.get_params().size() == 0)
		response = SERVER_NAME " 461 " + user->get_nickname() + " JOIN :Not enough parameters";
	
	// ERR_BANNEDFROMCHAN
	// ERR_INVITEONLYCHAN
	// ERR_BADCHANNELKEY
	// ERR_CHANNELISFULL 
	// ERR_BADCHANMASK
	// ERR_NOSUCHCHANNEL
	// ERR_TOOMANYCHANNELS
	// ERR_TOOMANYTARGETS
	// ERR_UNAVAILRESOURCE
	// RPL_TOPIC

	send_message(response, user->get_fd());
}
