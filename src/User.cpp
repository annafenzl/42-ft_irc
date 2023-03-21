/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afenzl <afenzl@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/19 13:39:50 by afenzl            #+#    #+#             */
/*   Updated: 2023/03/20 13:42:03 by afenzl           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../inc/User.hpp"

// --------------- constructors ---------------
User::User():_name("*"), _nickname("*"), _fullname("*"), _registered(false), _pass_provided(false) {}

User::User(int fd, char *hostmask)
	:_name("*"), _nickname("*"), _fullname("*"), _hostmask(hostmask), _fd(fd), _registered(false), _pass_provided(false) {}

// --------------- getters --------------------

std::string User::get_name() const		{ return _name; }

std::string User::get_nickname() const	{ return _nickname;}

std::string User::get_fullname() const	{ return _fullname;}

std::string User::get_hostmask() const	{ return _hostmask;}

std::string User::get_prefix() const	{ return ":" + _nickname + "!" + _name + "@" + _hostmask; }

int User::get_fd() const				{ return _fd;}

bool User::is_registered() const		{ return _registered;}

bool User::is_pass_provided() const		{ return _pass_provided;}

// --------------- setters --------------------

void User::set_name(std::string name )				{ _name = name; }

void User::set_nickname(std::string nickname)		{ _nickname = nickname; }

void User::set_fullname(std::string fullname)		{ _fullname = fullname; }

void User::set_registered(bool value)				{ _registered = value; }

void User::set_pass_provided(bool value)			{ _pass_provided = value; }

// --------------- methods -------------------

void User::append_buff(std::string input)
{
	buff.append(input);
}
