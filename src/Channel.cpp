/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katchogl <katchogl@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 11:22:22 by afenzl            #+#    #+#             */
/*   Updated: 2023/04/05 20:29:34 by katchogl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../inc/Channel.hpp"

/// ! constructors and destructor !
Channel::Channel( const std::string & name, const User & first_client )
{
	// TODO: edit client nickname
	_name = name;
	_clients.insert (_clients.end (), first_client);
	_creator = &(*_clients.begin ());
	_operators.insert (_clients.end (), first_client);
	_topic = "";
}

Channel::~Channel( void ) {}

/// ! getters !
const std::string &Channel::getName( void ) { return (_name ); }
User *Channel::getCreator( void ) { return (_creator ); }
const std::string &Channel::getTopic( void ) { return (_topic ); }

/// ! handlers !
int Channel::join( const User & client )
{
	
}

void Channel::topic( const std::string & topic ) { _topic = topic; }

/// ! static !
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

/// ! exceptions !
const char *Channel::InvalidChannelName::what( void ) const throw()
{
	return ("invalid class name");
}
