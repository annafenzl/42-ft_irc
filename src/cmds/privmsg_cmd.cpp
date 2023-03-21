/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg_cmd.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afenzl <afenzl@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 16:12:24 by afenzl            #+#    #+#             */
/*   Updated: 2023/03/21 12:34:20 by afenzl           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/Server.hpp"

//  Parameters: <msgtarget> <text to be sent>
void	Server::privmsg_command(Request request)
{
	std::string				response;
	User					*user = request.get_user();
	usermap::iterator		recipient;

	if (user->is_registered())
		response = SERVER_NAME " 462 " + user->get_nickname() + " :Unauthorized command (not yet registered)";

	// ERR_NORECIPIENT
	if (request.get_params().size() == 0 )
		response = SERVER_NAME " 411 " + user->get_nickname() + " :No recipient given PRIVSMG";

	// ERR_NOTEXTTOSEND
	else if (request.get_params().size() == 1)
		response = SERVER_NAME " 412 " + user->get_nickname() + " :No text to send";
	
	// ERR_NOSUCHNICK
	// need to think about mutlible users --> parse that!
	else if ((recipient = check_for_user(request.get_params()[0])) == _user_map.end())
		response = SERVER_NAME " 401 " + user->get_nickname()+ " " + request.get_params()[0] + " :No such nick/channel";

	// ERR_CANNOTSENDTOCHAN
	// ERR_NOTOPLEVEL	
		// 413   "<mask> :No toplevel domain specified"
	// ERR_WILDTOPLEVEL
		// 414 "<mask> :Wildcard in toplevel domain"
	// ERR_TOOMANYTARGETS
	// RPL_AWAY if the user is currently away

	else
	{
		send_message(user->get_prefix() + " PRIVMSG " + user->get_nickname() + " :" + request.get_params()[1], (*recipient).first);
		response = SERVER_NAME " 331 " + user->get_nickname() + " " + request.get_params()[0] + " :" + request.get_params()[1];
	}
	
	send_message(response, user->get_fd());

}
