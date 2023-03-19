/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afenzl <afenzl@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/19 16:08:06 by afenzl            #+#    #+#             */
/*   Updated: 2023/03/19 16:45:44 by afenzl           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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