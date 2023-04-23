/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annafenzl <annafenzl@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/19 16:08:06 by afenzl            #+#    #+#             */
/*   Updated: 2023/04/23 23:46:54 by annafenzl        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef REQUEST_HPP
# define REQUEST_HPP
# include <vector>
# include <string>
# include <iostream>
# include "User.hpp"

class Request
{
	private:
	std::string					_cmd;
	std::vector<std::string>	_params;

	User						*_user;
	std::string					_channel_name;
	std::string					_info;
	
	public:
	// --------- construct ---------
	Request(std::string input, User *user);

	// ---------- getters -----------
	std::string					get_cmd() const;
	std::vector<std::string>	get_params() const;
	User						*get_user() const;
	const 						std::string &get_channel_name( void ) const;
	const 						std::string &get_info( void ) const;
	
	// ---------- setters -----------
	void						set_user(User *user);
	void						set_info( const std::string & info );
	void						set_channel_name( const std::string & channelName );
	
	// ---------- debug ------------
	void print();
	
	// ---------- method -----------
	private:
	void parse(std::string(input));
};

# endif