/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annafenzl <annafenzl@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/12 17:06:05 by afenzl            #+#    #+#             */
/*   Updated: 2023/03/14 01:05:09 by annafenzl        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "server.hpp"

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

