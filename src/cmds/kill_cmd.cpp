/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kill_cmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pguranda <pguranda@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 11:18:44 by pguranda          #+#    #+#             */
/*   Updated: 2023/04/18 13:54:50 by pguranda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/Server.hpp"
# include <algorithm>

# include <sstream>
# include <set>

void Server::kill_command(Request request)
{
	std::string response;
	std::string user_to_kill = request.get_params()[0];
	std::string reason = request.get_params()[1];
	User* user = request.get_user();

	if (!user->is_registered()) {
		send_message(SERVER_NAME " 462 " + user->get_nickname() + " :Unauthorized command (not yet registered)", user->get_fd());
		return;
	}

	User* dead_user =  &check_for_user(user_to_kill)->second;
	if (!dead_user) {
		response = SERVER_NAME " 401 " + user_to_kill + " " + user_to_kill + " :No such nick/channel";
	}

	if (!user->is_operator()) {
		response = SERVER_NAME " 481 " + user->get_nickname() + " :Permission Denied- You're not an IRC operator";
	}

	if (reason.empty()) {
		response = SERVER_NAME " 461 " + user->get_nickname() + " :Not enough parameters";
	}

	if (!response.empty()) {
		send_message(response, user->get_fd());
		return;
	}

	std::string message = user_to_kill + " killed by " + user->get_nickname() + " (" + reason + ")";
	if (dead_user) {
		send_message(message, dead_user->get_fd());
		for (unsigned int i = 0; i < _fd_count; ++i) {
			if (_user_poll[i].fd == dead_user->get_fd()) {
				std::cout << "USER " << dead_user->get_nickname() << " hung up on " << dead_user->get_fd() << std::endl;
				remove_from_poll(i);
				break;
			}
		}
	for(channelmap::iterator it = _channels.begin(); it != _channels.end(); it++) 
		{
			std::list<User*>& members = it->second.getMembers(0);
			members.remove(dead_user);
		}
		_user_map.erase(dead_user->get_fd());
	} else 
	{
		send_message(message, user->get_fd());
	}
}
