/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kill_cmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pguranda <pguranda@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 11:18:44 by pguranda          #+#    #+#             */
/*   Updated: 2023/04/18 21:14:58 by pguranda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/Server.hpp"
# include <algorithm>

# include <sstream>
# include <set>

void Server::kill_command(Request request)
{
	std::string response;
	std::string user_to_kill = (request.get_params().size() == 0) ? "" : request.get_params()[0];
	std::string reason = (request.get_params().size() == 0) ? "" : request.get_params()[1];
	User* user = request.get_user();

	if (!user->is_registered()) 
	{
		send_message(SERVER_NAME " 462 " + user->get_nickname() + " :Unauthorized command (not yet registered)", user->get_fd());
		return;
	}

	User* dead_user =  &check_for_user(user_to_kill)->second;
	if (!dead_user) 
	{
		response = SERVER_NAME " 401 " + user_to_kill + " " + user_to_kill + " :No such nick/channel";
	}

	if (!user->is_operator()) {
		response = SERVER_NAME " 481 " + user->get_nickname() + " :Permission Denied- You're not an IRC operator";
	}

	if (reason.empty()) {
		response = SERVER_NAME " 461 " + user->get_nickname() + " :Not enough parameters (include nick and reason)";
	}

	if (!response.empty()) {
		send_message(response, user->get_fd());
		return;
	}

	std::string message = user_to_kill + " killed by " + user->get_nickname() + " (" + reason + ")";
	remove_user(dead_user, message);
}
