/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pguranda <pguranda@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/19 13:39:50 by afenzl            #+#    #+#             */
/*   Updated: 2023/04/14 15:07:53 by pguranda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../inc/User.hpp"

// --------------- constructors ---------------
User::User():_name("*"), _nickname("*"), _fullname("*"), _registered(false), _pass_provided(false), _channel(NULL) {}

User::User(int fd, char *hostmask)
	:_name("*"), _nickname("*"), _fullname("*"), _hostmask(hostmask), _fd(fd), _registered(false), _pass_provided(false), _channel(NULL) {}

User::User( const User & user ):
	_name(user.get_name ()), _nickname(user.get_nickname ()),
	_fullname(user.get_fullname ()), _hostmask(user.get_hostmask ()),
	_fd (user.get_fd ()), _registered(user.is_registered ()),
	_pass_provided (user.is_pass_provided ()), _channel(NULL) {}

// --------------- getters --------------------

std::string User::get_name() const		{ return _name; }

std::string User::get_nickname() const	{ return _nickname;}

std::string User::get_fullname() const	{ return _fullname;}

std::string User::get_hostmask() const	{ return _hostmask;}

std::string User::get_prefix() const	{ return ":" + _nickname + "!" + _name + "@" + _hostmask; }

int User::get_fd() const				{ return _fd;}

Channel *User::get_channel() const				{ return _channel; }

bool User::is_registered() const		{ return _registered;}

bool User::is_pass_provided() const		{ return _pass_provided;}

bool User::is_operator() const			{ return _operator_status;}

// --------------- setters --------------------

void User::set_name(std::string name )				{ _name = name; }

void User::set_nickname(std::string nickname)		{ _nickname = nickname; }

void User::set_fullname(std::string fullname)		{ _fullname = fullname; }

void User::set_registered(bool value)				{ _registered = value; }

void User::set_pass_provided(bool value)			{ _pass_provided = value; }

void User::set_operator(bool value)					{ _operator_status = value; }
void User::set_channel(Channel *channel)			{ _channel = channel; }

// --------------- methods -------------------

void User::append_buff(std::string input)
{
	buff.append(input);
}

bool operator==( const User & user, const User & user2 )
{
	if (user.get_nickname () == user2.get_nickname ())
		return (true);
	return (false);
}
