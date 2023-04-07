/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pguranda <pguranda@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 12:18:05 by afenzl            #+#    #+#             */
/*   Updated: 2023/04/07 18:22:21 by pguranda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

# include <iostream>

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
	
	public:
	// ------------- constructor -------------
	User();
	User(int fd, char *hostmask);

	// ------------- getters -----------------
	std::string get_name() const;
	std::string get_nickname() const;
	std::string get_fullname() const;
	std::string get_hostmask() const;
	std::string get_prefix() const;
	
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
	void set_operator(bool value);

	// ------------- methods -----------------

	void append_buff(std::string input);

};

#endif