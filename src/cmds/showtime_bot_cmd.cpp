/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   showtime_bot_cmd.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katchogl <katchogl@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/17 11:05:50 by pguranda          #+#    #+#             */
/*   Updated: 2023/04/18 23:39:04 by katchogl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

# include <sstream>
# include <set>
# include <ctime>
# include <time.h>
# include <iomanip>


void	Server::showtime_bot_command(Request request)
{
	std::cout << "SHOWTIME BOT COMMAND" << std::endl;
	Bot time_bot(-1, "BOT");
	User *user = request.get_user();
	if (!request.get_params().at(0).empty() && request.get_params()[0] == "BOT")
	{
		std::cout << "TIME IS: " << std::endl;
		time_t now = time(0);
		char *time_str;
		time_str = ctime(&now);

		std::string time_message = user->get_nickname() + ", the current time is: " + std::string(time_str);
		send_message(time_bot.get_prefix() + " PRIVMSG " + user->get_nickname() + " :" + time_message, user->get_fd());
		return;
	}
}
