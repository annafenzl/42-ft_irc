/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pguranda <pguranda@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid Date        by              +#+  #+#    #+#             */
/*   Updated: 2023/04/11 10:21:21 by pguranda         ###   ########.fr       */
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
# include "exit.hpp"
# include <list>
# include <algorithm>

class Channel
{
	private:
		std::string			_name;
		std::string			_topic;
		std::list<User *>	_members;
		std::string			_modes;
		
		/// ! constructors, copy assignment operator, destructor !
	public:
		Channel( void );
		Channel( const Channel & channel );
		Channel( const std::string & name );
		Channel &operator=( const Channel & channel );
		~Channel( void );
		
		/// ! basic getters !
		const std::string &getName( void ) const;
		const std::string &getTopic( void ) const;
		const std::string &getModes( void ) const;
		const std::list<User *> &getMembers( void ) const;
		std::list<User *> &getMembers( int );

		/// ! main !
		void setTopic( const std::string & topic );
		void join( User * member );
		void part( std::string name );

		/// ! static and utility !
		static bool isChannelCommand( const std::string & command );
		static bool isValidChannelName( const std::string & name );
		User *getMember( User *user );

		/// ! exceptions !
		class InvalidChannelName: public std::exception
		{
			public:
				const char	*what( void ) const throw();
		};
};
#endif
