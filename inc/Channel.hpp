/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pguranda <pguranda@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 18:15:17 by katchogl          #+#    #+#             */
/*   Updated: 2023/04/19 14:10:50 by pguranda         ###   ########.fr       */
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
# include "response.hpp"
# include <list>
# include <algorithm>

class Channel
{
	private:
		std::string			_name;
		std::string			_topic;
		std::string			_modes;
		std::string			_password;
		std::list<User *>	_members;
		std::list<User *>	_ops;
		
		/// ! constructors, copy assignment operator, destructor !
	public:
		Channel( void );
		Channel( const Channel & channel );
		Channel( const std::string & name, const std::string & password );
		Channel &operator=( const Channel & channel );
		~Channel( void );
		
		/// ! getters !
		const std::string &getName( void ) const;
		const std::string &getTopic( void ) const;
		const std::string &getModes( void ) const;
		const std::string &getPassword( void ) const;
		const std::list<User *> &getMembers( void ) const;
		std::list<User *> &getMembers( int );
		const std::list<User *> &getOps( void ) const;
		std::list<User *> &getOps( int );

		/// ! setters !
		void setTopic( const std::string & topic );

		/// ! container modifiers !
		void insert( User * user );
		int remove( User *user );
		void insertOp( User * op );
		void removeOp( User * op );
		void addMode (char m );
		void removeMode (char m );
		void removeMember( User *user );

		/// ! utility !
		static bool isValidChannelName( const std::string & name );
		bool isMember(User *user) const;
		bool isOp(User *user) const;
		bool hasMode (char m ) const;
		User *getMember( const std::string & nickame );
};
#endif
