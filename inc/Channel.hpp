/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pguranda <pguranda@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 18:15:17 by katchogl          #+#    #+#             */
/*   Updated: 2023/04/21 21:51:55 by pguranda         ###   ########.fr       */
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

# define CHANNEL_MODES "itkol"

enum Modes
{
	i_mode = 1,
	t_mode = 2,
	k_mode = 4,
	o_mode = 8,
	l_mode = 16,
};

class Channel
{
	private:
		std::string			_name;
		std::string			_topic;
		short				_modes;
		std::string			_password;
		std::list<User *>	_members;
		std::list<User *>	_ops;
		int					_limit;
		
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
		const std::list<User *> &getMembers( void ) const;
		std::list<User *> &getMembers( int );
		const std::list<User *> &getOps( void ) const;
		std::list<User *> &getOps( int );

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
		bool execMode (char mode, char sign, std::vector<std::string> params, unsigned int *i);

		/// ! utility !
		static bool isValidChannelName( const std::string & name );
		static bool isValidMode(char mode);
	
		bool isMember(User *user) const;
		bool isOp(User *user) const;
		bool hasMode (char mode ) const;
		
		User *getMember( const std::string & nickame );
		std::string getModeAsString( void ) const;
};
#endif
