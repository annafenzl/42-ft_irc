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
	typedef std::vector<User>	user_vect_t;
	
	private:
		std::string			_name;
		std::string			_topic;
		user_vect_t			members;

		// Constructors && Destructor
		Channel( void );
		Channel( const Channel & channel );
		Channel &operator=( const Channel & channel );

	public:
		Channel ( std::string name, std::string topic );
		~Channel( void );

		// Getters
		const std::string &getName( void );
		const std::string &getTopic( void );

		// Utility
};
