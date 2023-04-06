/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katchogl <katchogl@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 18:15:17 by katchogl          #+#    #+#             */
/*   Updated: 2023/04/06 13:22:16 by katchogl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "User.hpp"
# include <list>
# include <algorithm>
typedef int t_errno;

class Channel
{
	private:
		std::string			_name;
		std::string			_topic;
		std::list<User>		_members;
		
		Channel( void );
		Channel( const Channel & channel );
		Channel &operator=( const Channel & channel );

	public:
		~Channel( void );
		Channel( const std::string & name, const User & first_member );

		/// ! basic getters !
		const std::string &getName( void ) const;
		const std::string &getTopic( void ) const;

		/// ! main !
		t_errno topic( const User & self, const std::string & topic );
		int join( const User & member );
		void part( std::string name );

		/// ! static and utility !
		static bool isValidClassName( const std::string & className );
		User *getMember( const User & user );
		bool isBridged( const User & user );

		/// ! exceptions !
		class InvalidChannelName: public std::exception
		{
			public:
				const char	*what( void ) const throw();
		};
};

