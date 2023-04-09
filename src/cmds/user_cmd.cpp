/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user_cmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afenzl <afenzl@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 13:57:59 by afenzl            #+#    #+#             */
/*   Updated: 2023/04/06 11:08:47 by afenzl           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/Server.hpp"

//  do i need to check for forbiddden chars

/*
	Command: USER
	Parameters: <user> <mode> <unused> <realname>

	The USER command is used at the beginning of connection to specify
	the username, hostname and realname of a new user.
*/
void Server::user_command(Request request)
{
	std::string response;
	User		*user = request.get_user();


	if (user->is_pass_provided() == false)
		response.append(SERVER_NAME " 462 " + user->get_nickname() + " :Please provide the server password with PASS first");

	// ERR_ALREADYREGISTRED
	else if (user->is_registered() == true)
		response.append(SERVER_NAME " 462 " + user->get_nickname() + " :Unauthorized command (already registered)");

	//ERR_NEEDMOREPARAMS
	else if(request.get_params().size() != 4)
		response.append(SERVER_NAME " 461 " + user->get_nickname() + " USER :Not the right parameters");

	else
	{
		user->set_name(request.get_params()[0]);
		user->set_fullname(request.get_params()[3]);	
	}

	check_login_complete(user);

	if (!response.empty())
		send_message(response, user->get_fd());
}
