/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annafenzl <annafenzl@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 11:22:22 by afenzl            #+#    #+#             */
/*   Updated: 2023/04/18 16:14:42 by annafenzl        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../inc/Channel.hpp"

/// ! constructors and destructor !
Channel::Channel( void )
	: _name("*"), _topic("*"), _modes("nt"), _password("*")
{}

Channel::Channel( const std::string & name, const std::string & password )
	: _name(name), _topic("*"), _modes("nt"), _password(password)
{
	std::cout << "\033[0;32m[INFO] new channel: " + _name 
		+  ", with password: '" + _password + "' created\033[0m" << std::endl;
}

Channel &Channel::operator=( const Channel & channel )
{
	std::list<User *>::const_iterator it;

	if (this == & channel)
		return (*this);
	_name = channel.getName ();
	_password = channel.getPassword ();
	_topic = channel.getTopic ();
	_modes = channel.getModes ();
	_members = std::list<User *>();
	it = channel.getMembers ().begin ();
	while (it != channel.getMembers ().end ())
	{
		insert (*it);
		it++;
	}
	it = channel.getOps ().begin ();
	while (it != channel.getOps ().end ())
	{
		_ops.insert (_ops.end (), *it);
		it++;
	}
	return (*this);
}

Channel::Channel( const Channel & channel ):
	_name(channel.getName ()), _topic(channel.getTopic ()),
	_modes(channel.getModes ()), _password(channel.getPassword ())
{
	std::list<User *>::const_iterator it;
	
	it = channel.getMembers ().begin ();
	while (it != channel.getMembers ().end ())
	{
		insert (*it);
		it++;
	}
	it = channel.getOps ().begin ();
	while (it != channel.getOps ().end ())
	{
		_ops.insert (_ops.end (), *it);
		it++;
	}
}

Channel::~Channel( void ) {}

/// ! getters !
const std::string &Channel::getName( void ) const { return (_name ); }
const std::string &Channel::getPassword( void ) const { return (_password ); }
const std::string &Channel::getTopic( void ) const { return (_topic ); }
const std::string &Channel::getModes( void ) const { return (_modes); }

const std::list<User *> &Channel::getMembers( void ) const { return (_members); }
std::list<User *> &Channel::getMembers( int ) { return (_members); }
const std::list<User *> &Channel::getOps( void ) const { return (_ops); }
std::list<User *> &Channel::getOps( int ) { return (_ops); }

// User *Channel::getMember( User *user )
// {
// 	std::list<User *>::iterator it;

// 	it = std::find (_members.begin (), _members.end (), user);
// 	if (it != _members.end ())
// 		return (*it);
// 	return (NULL);
// }

User *Channel::getMember( const std::string & nickname )
{
	std::list<User *>::iterator it;

	it = _members.begin ();
	while (it != _members.end ())
	{
		if ((*it)->get_nickname () == nickname)
			return (*it);
		it++;
	}
	return (NULL);
}

User *Channel::getOp( User *op )
{
	std::list<User *>::iterator it;

	it = std::find (_ops.begin (), _ops.end (), op);
	if (it != _ops.end ())
		return (*it);
	return (NULL);
}

/// ! modifiers !
void Channel::setTopic( const std::string & topic ) { _topic = topic; }

void Channel::insert( User * user )
{
	if (getMember (user) == NULL)
		_members.insert (_members.end (), user);
		if (_members.size () == 1)
			_ops.insert (_ops.end (), user);
}

/*
	removes user from _members
	returns true if there is no member left
*/
int Channel::remove( User * user )
{
	_members.remove(user);
	return(_members.empty());
}

void Channel::insertOp( User * op )
{
	if (getOp (op) == NULL)
	{
		_ops.insert (_ops.end (), op);
		if (_ops.size () == 1)
			_ops.insert (_ops.end (), op);
	}
}

void Channel::removeOp( User * op )
{
	_ops.remove(op);
}

void Channel::addMode( char m )
{
	size_t i;

	i = 0;
	while (_modes[i])
	{
		if (_modes[i] == m)
			break ;
		i++;
	}
	if (i == _modes.length ())
		_modes += m;
}

void Channel::removeMode( char m )
{
	size_t i;
	std::string nModes;

	i = 0;
	while (_modes[i])
	{
		if (_modes[i] != m)
			nModes += _modes[i];
		i++;
	}
	_modes = nModes;
}

/// ! utility !
bool	Channel::isValidChannelName( const std::string & name )
{
	int i;

	if (name.empty () || (name[0] != '#' && name[0] != '&'))
		return (false);
	i = -1;
	while (name[++i])
		if (name[i] == ' ' || name[i] == ','
			|| name[i] == 7) // control G/ BELL
			return (false);
	return (true);
}

User *Channel::getMember( User *user )
{
	std::list<User *>::iterator it;

	it = std::find (_members.begin (), _members.end (), user);
	if (it != _members.end ())
		return (*it);
	return (NULL);
}

/*
	returns true if the passed User is part of the Channel
*/
bool Channel::isMember( User *user ) const
{
	return (find (_members.begin (), _members.end (), user) != _members.end ());
}

void Channel::removeMember(User *user)
{
	_members.remove(user);
}



bool Channel::hasMode( char m )
{
	size_t i;
	std::string nModes;

	i = 0;
	while (_modes[i])
	{
		if (_modes[i] == m)
			return (true);
		i++;
	}
	return (false);
}
