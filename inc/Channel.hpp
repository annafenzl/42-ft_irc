/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katchogl <katchogl@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 18:15:17 by katchogl          #+#    #+#             */
/*   Updated: 2023/04/05 20:28:25 by katchogl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "User.hpp"
# include <list>

class Channel
{
	private:
		std::string			_name;
		std::string			_topic;
		std::list<User>		_clients;
		std::list<User>		_operators;
		User				*_creator;
		// TODO: modes
		
		Channel( void );
		Channel( const Channel & channel );
		Channel &operator=( const Channel & channel );

	public:
		~Channel( void );
		Channel( const std::string & name, const User & first_client );

		/// ! getters !
		const std::string &getName( void );
		User *getCreator( void );
		const std::string &getTopic( void );

		/// ! handlers !
		int join( const User & client );
		void part( std::string name );
		void topic( const std::string & topic );

		/// ! static !
		static bool isValidClassName( const std::string & className ); // TODO: utilize

		/// ! exceptions !
		class InvalidChannelName: public std::exception
		{
			public:
				const char	*what( void ) const throw();
		};
};

