/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afenzl <afenzl@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/19 16:08:06 by afenzl            #+#    #+#             */
/*   Updated: 2023/03/19 16:11:17 by afenzl           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <vector>
# include <string>
# include <iostream>

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
