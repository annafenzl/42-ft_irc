/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   notice_cmd.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pguranda <pguranda@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 15:22:07 by annafenzl         #+#    #+#             */
/*   Updated: 2023/04/07 17:12:07 by pguranda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/Server.hpp"

# include <sstream>
# include <set>

std::set<std::string> split_targets(std::string targets, std::string &duplicate);

void	Server::notice_command(Request request)
{
	User *user = request.get_user();
	usermap::iterator recipient;

	// Do not process the command if the user is not registered
	if (!user->is_registered())
		return;

	// Do not process the command if no recipient or text is provided
	if (request.get_params().size() < 2)
		return;

	std::string duplicate;
	std::set<std::string> targets = split_targets(request.get_params()[0], duplicate);

	// Do not send the message if there is a duplicate recipient
	if (duplicate.empty() == false)
		return;

	for (std::set<std::string>::iterator it = targets.begin(); it != targets.end(); ++it)
	{
		if ((recipient = check_for_user(*it)) == _user_map.end())
			continue;

		send_message(user->get_prefix() + " NOTICE " + *it + " :" + request.get_params()[1], recipient->first);
	}
}