/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annafenzl <annafenzl@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/19 14:14:39 by afenzl            #+#    #+#             */
/*   Updated: 2023/04/24 00:29:59 by annafenzl        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../inc/Request.hpp"

// ------------- constructor -------------
Request::Request(std::string input, User *user): _user(user), _channel_name("*"), _info("*")
{
	parse(input);
}

// ------------- getters ----------------

std::string					Request::get_cmd() const		{ return _cmd; }

std::vector<std::string>	Request::get_params() const		{ return _params; }

User						*Request::get_user() const		{ return _user; }

const 						std::string &Request::get_channel_name( void ) const { return _channel_name; }

const 						std::string &Request::get_info( void ) const { return _info; }

// ------------- setters ----------------

void						Request::set_user(User *user)	{ _user = user; }

void						Request::set_channel_name( const std::string & channelName ) { _channel_name = channelName; }

void						Request::set_info( const std::string & info ) { _info = info; }

// ------------- methods ----------------

/*
	parses the input to:
	- _comd (the first string until ' '(space))
	- _params (strings divided by ' '(space), unless it's preceeded by a ':'(colon))
*/
void Request::parse(std::string input)
{
	std::cout << "\n\n\n-> Parsing |" << input << "| <-" << std::endl;

	// skip whitespace
	input = input.substr(input.find_first_not_of(" "));
	// find command
	size_t space_pos = input.find_first_of(" ");

	_cmd = input.substr(0, space_pos);
	
	////////////////////////////////////////////////////////////
	/// assert that the command is always in uppercase
	////////////////////////////////////////////////////////////
	size_t i;
	i = 0;
	while (i < _cmd.size ())
	{
		if (std::islower (_cmd[i]))
			_cmd[i] = std::toupper (_cmd[i]);
		i++;
	}
	
	if (space_pos != std::string::npos)
		++space_pos;
	input.erase(0, space_pos);
	// insert params
	while (!input.empty())
	{
		input = input.substr(input.find_first_not_of(" "));
		if (input[0] == ':')
		{
			input.erase(input.begin());
			if (!input.empty())
				_params.push_back(input);
			break;
		}
		space_pos = input.find_first_of(" ");
		if (space_pos == std::string::npos)
		{
			_params.push_back(input);
			break;
		}
		else
		{
			_params.push_back(input.substr(0, space_pos));
			input.erase(0, ++space_pos);
		}
	}

}

// ------------- debug ----------------

void Request::print()
{
	std::cout << "--------- after parsing ----------" << std::endl;
	std::cout << "CMD: |" << _cmd << "|" << std::endl;
	std::cout << "PARAMS: ";
	for (std::vector<std::string>::iterator it = _params.begin(); it != _params.end(); ++it)
		std::cout << "<" << *it << "> ";
	std::cout << "\n----------------------------------" << std::endl;
}