/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */

/*   By: katchogl <katchogl@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 11:22:22 by afenzl            #+#    #+#             */
/*   Updated: 2023/04/19 00:04:43 by katchogl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../inc/Channel.hpp"

/// ! constructors and destructor !
Channel::Channel( void )
	: _name("*"), _topic("*"), _modes(t_mode), _password("*"), _limit(-1)
{}

Channel::Channel( const std::string & name, const std::string & password )
	: _name(name), _topic("*"), _modes(t_mode), _password(password), _limit(-1)
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
	_modes(channel.getModes ()), _password(channel.getPassword ()), _limit(channel.getLimit())
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
int		Channel::getLimit( void ) const { return (_limit); }
short	Channel::getModes( void ) const { return (_modes); }
const std::string &Channel::getName( void ) const { return (_name ); }
const std::string &Channel::getTopic( void ) const { return (_topic ); }
const std::string &Channel::getPassword( void ) const { return (_password ); }
const std::list<User *> &Channel::getMembers( void ) const { return (_members); }
std::list<User *> &Channel::getMembers( int ) { return (_members); }
const std::list<User *> &Channel::getOps( void ) const { return (_ops); }
std::list<User *> &Channel::getOps( int ) { return (_ops); }

/// ! setters !
void Channel::setLimit( int limit ) { _limit = limit; }
void Channel::setTopic( const std::string & topic ) { _topic = topic; }
void Channel::setPassword( const std::string & password ) { _password= password; }

/// ! container modifiers !
void Channel::insert( User * user )
{
	if (!isMember (user))
	{
		_members.insert (_members.end (), user);
		if (_members.size () == 1)
			insertOp (user);
	}
}

int Channel::remove( User * user )
{
	_members.remove(user);
	_ops.remove(user);
	return(_members.empty());
}

void Channel::insertOp( User * op )
{
	if (!isOp (op))
		_ops.insert (_ops.end (), op);
}

void Channel::removeOp( User * op )
{
	_ops.remove(op);
}

short ModeToBitmask(char mode)
{
	short mode_switch;

	if (mode == 'i')
		mode_switch = i_mode;
	else if (mode == 't')
		mode_switch = t_mode;
	else if (mode == 'k')
		mode_switch = k_mode;
	else if (mode == 'o')
		mode_switch = o_mode;
	else if (mode == 'l')
		mode_switch = l_mode;
	else
		mode_switch = 0;
	
	return mode_switch;
}

void Channel::editMode(char mode, char sign)
{
	short update_mode = ModeToBitmask(mode);

	if (sign == '+')
		_modes |= update_mode;
	else if (sign == '-')
		_modes &= ~update_mode;
}

/// ! utility !
bool	Channel::isValidChannelName( const std::string & name )
{
	int i;

	if (name.empty () || (name[0] != '#'))
		return (false);
	i = -1;
	while (name[++i])
		if (name[i] == ' ' || name[i] == ','
			|| name[i] == 7) // control G/ BELL
			return (false);
	return (true);
}

bool Channel::isValidMode(char mode)
{
	return (std::string(CHANNEL_MODES).find_first_of(mode) != std::string::npos);
}


bool Channel::isMember( User *user ) const
{
	return (find (_members.begin (), _members.end (), user) != _members.end ());
}

bool Channel::isOp( User *user ) const
{
	return (find (_ops.begin (), _ops.end (), user) != _ops.end ());
}

bool Channel::hasMode( char mode ) const
{
	return (_modes & ModeToBitmask(mode));
}

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

std::string Channel::getModeAsString( void ) const
{
	std::string mode_string = "+";

	if (_modes & i_mode)
		mode_string.append("i");
	if (_modes & t_mode)
		mode_string.append("t");
	if (_modes & k_mode)
		mode_string.append("k");
	if (_modes & o_mode)
		mode_string.append("o");
	if (_modes & l_mode)
		mode_string.append("l");

	return mode_string;
}

bool Channel::execMode(char mode, char sign, std::vector<std::string> params, unsigned int * i)
{
	// also need to reverse settings in case of '-'
	// need to find a way to handle flags with params (k and l are called at the same time

	// invite only
	if (mode == 'i')
	{
		std::cout << "got i" << std::endl;
		return true;
	}
	// topic
	else if (mode == 't')
	{
		std::cout << "got l" << std::endl;
		if (sign == '-')
		{
			setTopic("no topic set");
			return true;
		}
		else if (params.size() - 1 > *i)
		{
			setTopic(params[++(*i)]);
			return true;
		}
	}
	// operator
	else if (mode == 'o')
	{
		std::cout << "got o" << std::endl;
		return true;
	}
	// key
	else if (mode == 'k')
	{
		std::cout << "got k" << std::endl;
		if (sign == '-')
		{
			setPassword("*");
			return true;
		}
		else if(params.size() - 1 > *i)
		{
			setPassword(params[++(*i)]);
			return true;
		}
	}
	// limit
	else if (mode == 'l')
	{
		std::cout << "got l" << std::endl;
		if (sign == '-')
		{
			setLimit(-1);
			return true;
		}
		else if (params.size() - 1 > *i)
		{
			int update_limit = strtol(params[++(*i)].c_str(), NULL, 10);
			if (update_limit > static_cast<int>(_members.size()))
			{
				_limit = update_limit;
				std::cout << "set limit to " << _limit << std::endl;
				return true;
			}
		}
	}
	return false;
}