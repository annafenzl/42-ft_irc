/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afenzl <afenzl@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 11:22:48 by afenzl            #+#    #+#             */
/*   Updated: 2023/03/21 12:33:58 by afenzl           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "User.hpp"
# include <vector>

class Channel
{
	private:
	std::string			_channelname;
	std::string			_topic;
	std::vector<User>	_user;
	
	public:
	
};