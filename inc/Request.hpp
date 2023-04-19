/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katchogl <katchogl@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/19 16:08:06 by afenzl            #+#    #+#             */
/*   Updated: 2023/04/19 01:32:49 by katchogl         ###   ########.fr       */
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
	void						set_user(User *user);
	void						set_channel_name( const std::string & channelName );
	const 						std::string &get_channel_name( void ) const;
	void						set_info( const std::string & info );
	const 						std::string &get_info( void ) const;
	
	// ---------- debug ------------
	void print();
	
	// ---------- method -----------
	private:
	void parse(std::string(input));
};

# endif