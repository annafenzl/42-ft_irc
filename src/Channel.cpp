/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katchogl <katchogl@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 11:22:22 by afenzl            #+#    #+#             */
/*   Updated: 2023/04/06 13:23:48 by katchogl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../inc/Channel.hpp"

/// ! constructors and destructor !
Channel::Channel( const std::string & name, const User & first_member )
{
	// TODO: edit member nickname
	_name = name;
	_members.insert (_members.end (), first_member);
	_members.begin()->first.set
	_topic = "";
}

Channel::~Channel( void ) {}

/// ! basic getters !
const std::string &Channel::getName( void ) const { return (_name ); }
const std::string &Channel::getTopic( void ) const { return (_topic ); }

/// ! handlers !
int Channel::topic( const User & self, const std::string & topic )
{
	return (0);
}

int Channel::join( const User & member )
{
	
	return (0);
}

/// ! static and utility !
bool	Channel::isValidClassName( const std::string & className )
{
	int i;

	if (className[0] != '#' && className[0] != '&')
		return (false);
	i = -1;
	while (className[++i])
		if (className[i] == ' ' || className[i] == ','
			|| className[i] == 7) // control G/ BELL
			return (false);
	return (true);
}

User *Channel::getMember( const User & user )
{
	std::list<User>::iterator it;

	it = std::find (_members.begin (), _members.end (), user);
	if (it != _members.end ())
		return (&(*it));
	return (NULL);
}

bool	Channel::isBridged( const User & user ) { return (getMember (user) != NULL); }

/// ! exceptions !
const char *Channel::InvalidChannelName::what( void ) const throw()
{
	return ("invalid class name");
}
