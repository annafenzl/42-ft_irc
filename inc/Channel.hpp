/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annafenzl <annafenzl@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 18:15:17 by katchogl          #+#    #+#             */
/*   Updated: 2023/04/24 00:21:16 by annafenzl        ###   ########.fr       */
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
# include <vector>
# include <algorithm>
# include "Request.hpp"

# define CHANNEL_MODES "itkol"
# define ARG_CHANNEL_MODES "kol"

enum Modes
{
	i_mode = 1,
	t_mode = 2,
	k_mode = 4,
	o_mode = 8,
	l_mode = 16,
};

class Server ;

class Channel
{
	public:
	typedef std::list<User *> userlist;

	private:
		std::string		_name;
		std::string		_topic;
		std::string		_password;

		userlist		_members;
		userlist		_ops;

		short			_modes;
		int				_limit;
		
		/// ! constructors, copy assignment operator, destructor !
	public:
		Channel( void );
		Channel( const Channel & channel );
		Channel( const std::string & name, const std::string & password );
		Channel &operator=( const Channel & channel );
		~Channel( void );
		
		/// ! getters !
		short getModes( void ) const;
		int   getLimit( void ) const;

		const std::string &getName( void ) const;
		const std::string &getTopic( void ) const;
		const std::string &getPassword( void ) const;

		const userlist &getMembers( void ) const;
		userlist &getMembers( int );

		const userlist &getOps( void ) const;
		userlist &getOps( int );

		/// ! setters !
		void setTopic( const std::string & topic );
		void setLimit( int limit );
		void setPassword( const std::string & password );

		/// ! container modifiers !
		void insert( User * user );
		int remove( User *user );

		void insertOp( User * op );
		void removeOp( User * op );

		void editMode (char mode, char sign);
		bool execMode (char mode, char sign, std::string param, const Server & server, Request request);

		/// ! utility !
		static bool isValidChannelName( const std::string & name );
		static bool isValidMode(char mode);
		static bool isValidParamCase(char mode, char sign);

		bool isMember(User *user) const;
		bool isOp(User *user) const;
		bool hasMode (char mode ) const;
		
		User *getMember( const std::string & nickame );
		std::string getModeAsString( void ) const;
};
#endif
