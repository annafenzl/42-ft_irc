/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pguranda <pguranda@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 11:22:22 by afenzl            #+#    #+#             */
/*   Updated: 2023/04/07 13:50:38 by pguranda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../inc/Channel.hpp"

Channel::Channel(Request &request)
{
	_channelname = request.get_params()[0];
	_users.push_back(*request.get_user());
}

Channel::~Channel()
{
}

std::string	Channel::get_name()
{
	return _channelname;
}

std::list<User>	Channel::get_users()
{
	return _users;
}

void	Channel::add_user(User *user)
{
	// _users.push_back(user);
}

void	Channel::remove_user(User *user)
{
	// std::vector<User*>::iterator it = _users.begin();
	// while (it != _users.end())
	// {
	// 	if (*it == user)
	// 	{
	// 		_users.erase(it);
	// 		return ;
	// 	}
	// 	it++;
	// }
}
