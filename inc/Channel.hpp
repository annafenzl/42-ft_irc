/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katchogl <katchogl@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 18:15:17 by katchogl          #+#    #+#             */
/*   Updated: 2023/03/22 18:24:35 by katchogl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "User.hpp"
# include <vector>

class Channel
{
	private:
		std::string			_name;
		std::string			_topic;
		short				_modes;
		// int					_limit;
		
		std::list<User>		_users;
		// std::list<User>		_operators;
		// std::list<User>		_banned;
		
	public:
		// ------------- constructor --------------
		Channel();
		Channel(std::string name, User first_user);
		
		// --------------- getters ----------------
		// --------------- setters ----------------
		// --------------- methods ----------------
		
};

