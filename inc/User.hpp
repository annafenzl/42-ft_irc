/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pguranda <pguranda@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid Date        by              +#+  #+#    #+#             */
/*   Updated: 2023/04/11 13:38:13 by pguranda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#ifndef USER_HPP
# define USER_HPP

# include <iostream>

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

		Channel			*_channel;
		
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
		Channel * get_channel() const;
		
		int get_fd() const;

		bool is_registered() const;
		bool is_pass_provided() const;
	bool is_operator() const;

		// ------------- setters -----------------
		
		void set_name(std::string name);
		void set_nickname(std::string nickname);
		void set_fullname(std::string fullname);

		void set_registered(bool value);
		void set_pass_provided(bool value);
		void set_channel(Channel *channel);
		void set_operator(bool value);

		// ------------- methods -----------------

		void append_buff(std::string input);
};

bool operator==( const User & user, const User & user2 );
#endif
