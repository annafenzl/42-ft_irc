/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pguranda <pguranda@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/19 16:08:06 by afenzl            #+#    #+#             */
/*   Updated: 2023/04/07 13:48:05 by pguranda         ###   ########.fr       */
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
	
	public:
	// --------- construct ---------
	Request(std::string input, User *user);

	// ---------- getters -----------
	std::string					get_cmd() const;
	std::vector<std::string>	get_params() const;
	User						*get_user() const;
	
	// ---------- debug ------------
	void print();
	
	// ---------- method -----------
	private:
	void parse(std::string(input));
};

# endif