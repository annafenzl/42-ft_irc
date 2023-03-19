/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afenzl <afenzl@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/19 14:14:39 by afenzl            #+#    #+#             */
/*   Updated: 2023/03/19 17:07:07 by afenzl           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../inc/Request.hpp"

// ------------- constructor -------------
Request::Request(std::string input, User *user): _user(user)
{
	parse(input);
}

// ------------- getters ----------------

std::string					Request::get_cmd() const		{ return _cmd; }

std::vector<std::string>	Request::get_params() const		{ return _params; }

User						*Request::get_user() const		{ return _user; }

// ------------- methods ----------------

void Request::parse(std::string input)
{
	std::cout << "\n\n\n-> Parsing |" << input << "| <-" << std::endl;
	
	// skip whitespace
	input = input.substr(input.find_first_not_of(" "));
	// find command
	int space_pos = input.find_first_of(" ");
	_cmd = input.substr(0, space_pos);
	if (space_pos != std::string::npos)
		++space_pos;
	input.erase(0, space_pos);
	// insert params
	while (!input.empty())
	{
		input = input.substr(input.find_first_not_of(" "));
		space_pos = input.find_first_of(" ");
		if (space_pos == std::string::npos)
		{
			_params.push_back(input);
			break;
		}
		else
		{
			if (input[0] == ':')
			{
				_params.push_back(input);
				break;
			}
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