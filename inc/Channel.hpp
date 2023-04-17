/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afenzl <afenzl@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 18:15:17 by katchogl          #+#    #+#             */
/*   Updated: 2023/04/17 14:50:11 by afenzl           ###   ########.fr       */
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
		
		/// ! getters !
		const std::string &getName( void ) const;
		const std::string &getTopic( void ) const;
		const std::string &getModes( void ) const;
		const std::list<User *> &getMembers( void ) const;
		std::list<User *> &getMembers( int );

		/// ! setters !
		void setTopic( const std::string & topic );
		void insert( User * user );

		/// ! utility !
		bool isMember(User *user) const;
		static bool isValidChannelName( const std::string & name );
		void removeMember(User *user);

		/// ! exceptions !
		class InvalidChannelName: public std::exception
		{
			public:
				const char	*what( void ) const throw();
		};
};
#endif
