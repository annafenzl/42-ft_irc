/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afenzl <afenzl@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 15:48:33 by pguranda          #+#    #+#             */
/*   Updated: 2023/04/16 14:24:09 by afenzl           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../inc/Bot.hpp"

Bot::Bot(int fd, const std::string& hostmask) : User(fd, const_cast<char*>(hostmask.c_str())) 
{
	set_nickname("BOT");
	set_fullname("BOT");
	set_registered(true);
}

Bot::~Bot()
{
}

//The main implementaiton of the bot commands is in privmsg - as it relates to privmsg handling