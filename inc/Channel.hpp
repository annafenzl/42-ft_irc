/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pguranda <pguranda@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 18:15:17 by katchogl          #+#    #+#             */
/*   Updated: 2023/04/18 09:34:18 by pguranda         ###   ########.fr       */
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
		std::string			_password;
		std::string			_topic;
		std::list<User *>	_members;
		std::list<User *>	_ops;
		std::string			_modes;
		
		/// ! constructors, copy assignment operator, destructor !
	public:
		Channel( void );
		Channel( const Channel & channel );
		Channel( const std::string & name, const std::string & password );
		Channel &operator=( const Channel & channel );
		~Channel( void );
		
		/// ! getters !
		const std::string &getName( void ) const;
		const std::string &getPassword( void ) const;
		const std::string &getTopic( void ) const;
		const std::string &getModes( void ) const;
		
		const std::list<User *> &getMembers( void ) const;
		std::list<User *> &getMembers( int );
		const std::list<User *> &getOps( void ) const;
		std::list<User *> &getOps( int );
		
		User *getMember( User *user );
		User *getMember( const std::string & nickame );
		User *getOp( User *op );

		/// ! modifiers !
		void setTopic( const std::string & topic );
		void insert( User * user );
		void remove( User *user );
		void insertOp( User * op );
		void removeOp( User * op );
		void addMode (char m );
		void removeMode (char m );

		/// ! utility !
		static bool isValidChannelName( const std::string & name );
		bool isOp( User * user );

		bool hasMode (char m );
};
#endif
