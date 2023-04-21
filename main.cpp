/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pguranda <pguranda@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/12 17:06:05 by afenzl            #+#    #+#             */
/*   Updated: 2023/04/19 18:12:26 by pguranda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "inc/User.hpp"
# include "inc/Server.hpp"

int	main(int argc, char **argv)
{
	try
	{
		if (argc != 3)
			throw std::invalid_argument("please provide <port> <password>");
		Server server(argv);
		std::cout << "port: " << server.get_port() << "\npassword: " << server.get_password() << std::endl;
		server.run();
	}
	catch(const std::exception& e)
	{
		std::cerr << "\033[0;31m" << e.what() << "\033[0m" << '\n';
		return (1);
	}
	return(0);
}

