/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pguranda <pguranda@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 12:18:05 by afenzl            #+#    #+#             */
/*   Updated: 2023/04/18 10:17:15 by pguranda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

# include <iostream>
# include <map>

class Channel;

class User
{
	public:
		std::string		buff;

	private:	
		std::string		_name;
		std::string		_nickname;
		std::string 	_fullname;
		std::string		_hostmask;

		int				_fd;

		bool			_registered;
		bool			_pass_provided;

		bool			_operator_status;

		std::map<std::string, Channel *> _channels;
		
	public:
		// ------------- constructor -------------
		User();
		User( const User & user );
		User(int fd, char *hostmask);

		// ------------- getters -----------------
		std::string get_name() const;
		std::string get_nickname() const;
		std::string get_fullname() const;
		std::string get_hostmask() const;
		std::string get_prefix() const;
		const std::map<std::string, Channel *> &getChannels ( void ) const;
		std::map<std::string, Channel *> &getChannels( int );
		
		int get_fd() const;

		bool is_registered() const;
		bool is_pass_provided() const;
		bool is_operator() const;

		// ------------- setters -----------------
		
		void set_name(std::string name);
		void set_nickname(std::string nickname);
		void set_fullname(std::string fullname);
		void set_operator(bool value);
		void set_registered(bool value);
		void set_pass_provided(bool value);

		// ------------- methods -----------------

		void append_buff(std::string input);
};

bool operator==( const User & user, const User & user2 );
#endif
