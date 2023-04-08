/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pguranda <pguranda@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 15:48:33 by pguranda          #+#    #+#             */
/*   Updated: 2023/04/08 16:06:10 by pguranda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../inc/Bot.hpp"
#include <ctime>


Bot::Bot(int fd, const std::string& hostmask) : User(fd, const_cast<char*>(hostmask.c_str())) 
{
		set_nickname("Bot");
		set_fullname("Bot");
		set_registered(true);
	}

Bot::~Bot()
{
}
