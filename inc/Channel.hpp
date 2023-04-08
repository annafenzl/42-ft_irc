/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katchogl <katchogl@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 18:15:17 by katchogl          #+#    #+#             */
/*   Updated: 2023/04/08 20:05:37 by katchogl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

////////////////////////////////////////////////////////////
/// Class model representing an IRC channel
/// , featuring all its attributes such as a name, a topic
/// and handling its commands such as JOIN, TOPIC.
////////////////////////////////////////////////////////////

#ifndef CHANNEL_HPP
# define CHANNEL_HPP
# include "User.hpp"
# include "errco.hpp"
# include <list>
# include <algorithm>

class Channel
{
	private:
		std::string			_name;
		std::string			_topic;
		std::list<User>		_members;
		std::string			_modes;
		
		/// ! constructors && destructor !
		Channel( void );
		Channel &operator=( const Channel & channel );

	public:
		~Channel( void );
		Channel( const Channel & channel );
		Channel( const std::string & name, const User & first_member );

		/// ! basic getters !
		const std::string &getName( void ) const;
		const std::string &getTopic( void ) const;
		const std::string &getModes( void ) const;
		const std::list<User> &getMembers( void ) const;

		/// ! main !
		int topic( const User & self, const std::string & topic );
		int join( const User & member );
		void part( std::string name );

		/// ! static and utility !
		static bool isChannelCommand( const std::string & command );
		static bool isValidChannelName( const std::string & name );
		User *getMember( const User & user );
		bool isBridged( const User & user );

		/// ! exceptions !
		class InvalidChannelName: public std::exception
		{
			public:
				const char	*what( void ) const throw();
		};
};
#endif