/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katchogl <katchogl@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 11:22:22 by afenzl            #+#    #+#             */
/*   Updated: 2023/04/10 01:39:27 by katchogl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../inc/Channel.hpp"

/// ! constructors and destructor !
Channel::Channel( void )
	: _name("*"), _topic("*"), _modes("*")
{}

Channel::Channel( const std::string & name )
	: _name(name), _topic("*"), _modes("*")
{}

Channel &Channel::operator=( const Channel & channel )
{
	std::list<User *>::const_iterator it;

	if (this == & channel)
		return (*this);
	_name = channel.getName ();
	_topic = channel.getTopic ();
	_modes = channel.getModes ();
	_members = std::list<User *>();
	it = channel.getMembers ().begin ();
	while (it != channel.getMembers ().end ())
	{
		join (*it);
		it++;
	}
	return (*this);
}

Channel::Channel( const Channel & channel ):
	_name(channel.getName ()), _topic(channel.getTopic ()),
	_modes(channel.getModes ())
{
	std::list<User *>::const_iterator it;
	
	it = channel.getMembers ().begin ();
	while (it != channel.getMembers ().end ())
	{
		join (*it);
		it++;
	}
}

Channel::~Channel( void ) {}

/// ! basic getters !
const std::string &Channel::getName( void ) const { return (_name ); }
const std::string &Channel::getTopic( void ) const { return (_topic ); }
const std::string &Channel::getModes( void ) const { return (_modes); }
const std::list<User *> &Channel::getMembers( void ) const { return (_members); }
std::list<User *> &Channel::getMembers( int ) { return (_members); }

/// ! handlers !
void Channel::setTopic( const std::string & topic ) { _topic = topic; }

void Channel::join( User * member )
{
	_members.insert (_members.end (), member);
}

/// ! static and utility !
bool Channel::isChannelCommand( const std::string & command )
{
	if (command == "JOIN" || command == "NAMES"
		|| command == "LIST" || command == "TOPIC"
		|| command == "PART")
		return (true);
	return (false);
}

bool	Channel::isValidChannelName( const std::string & name )
{
	int i;

	if (name[0] != '#' && name[0] != '&')
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

/// ! exceptions !
const char *Channel::InvalidChannelName::what( void ) const throw()
{
	return ("invalid class name");
}
