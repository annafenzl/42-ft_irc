/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afenzl <afenzl@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 12:18:05 by afenzl            #+#    #+#             */
/*   Updated: 2023/03/19 15:26:52 by afenzl           ###   ########.fr       */
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
	
	public:
	// ------------- constructor -------------
	User();
	User(int fd, char *hostmask);

	// ------------- getters -----------------
	std::string get_name() const;
	std::string get_nickname() const;
	std::string get_fullname() const;
	std::string get_hostmask() const;
	
	int			get_fd() const;

	// ------------- setters -----------------
	
	// void set_name(std::string name);
	// void set_nickname(std::string nickname);
	// void set_fullname(std::string fullname);

	// ------------- methods -----------------

	void append_buff(std::string input);
	
};

# include <vector>

# define END_SEQUENCE "\r\n"

class Request
{
	private:
	std::string					_cmd;
	std::vector<std::string>	_params;
	
	public:
	// --------- construct ---------
	Request(std::string input);
	
	// ---------- debug ------------
	void print();

	private:
	
};



#endif