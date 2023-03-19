/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afenzl <afenzl@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/19 13:39:50 by afenzl            #+#    #+#             */
/*   Updated: 2023/03/19 15:27:04 by afenzl           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "User.hpp"

// --------------- constructors ---------------
User::User():_name("*"), _nickname("*"), _fullname("*") {}

User::User(int fd, char *hostmask)
	:_name("*"), _nickname("*"), _fullname("*"), _hostmask(hostmask), _fd(fd) {}

// --------------- getters --------------------

std::string User::get_name() const		{ return _name; }

std::string User::get_nickname() const	{ return _nickname;}

std::string User::get_fullname() const	{ return _fullname;}

std::string User::get_hostmask() const	{ return _hostmask;}

int			User::get_fd() const		{ return _fd;}

// --------------- setters --------------------

// void User::set_name(std::string name )		{ _name = name; }
// void User::set_nickname(std::string nickname)	{ _nickname = nickname; }
// void User::set_fullname(std::string fullname)	{ _fullname = fullname; }

// --------------- methods -------------------

void User::append_buff(std::string input)
{
	buff.append(input);
}
