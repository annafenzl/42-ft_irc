/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: annafenzl <annafenzl@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 00:13:32 by annafenzl         #+#    #+#             */
/*   Updated: 2023/03/14 06:50:00 by annafenzl        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "server.hpp"


// -------------- Constructor ------------------
Server::Server(char **argv)
{
	// PORTS WITH 878whatever are still valid!!!!
	long	port = strtol(argv[1], NULL, 0);
	if (port < 1 || port > 65535)
		throw IncorrectPortNumber();
	_port = (int) port;
	_password = argv[2];
	// are there other passwords that are invalid
	if (_password.empty())
		throw InvalidPassword();
}

// -------------- Getters ----------------------
int	Server::get_port()
{
	return _port;
}

std::string	Server::get_password()
{
	return _password;
}

// -------------- Methods ----------------------

void Server::run()
{
	int		sock_fd, n;
	int		sendbytes;
	struct	sockaddr_in	servaddr;
	char	sendline[MAXLINE];
	char	recvline[MAXLINE];

	if ( (sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) // using TCP as default for stram sockets
		SocketError();

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET; //Address Family Internet
	servaddr.sin_port = htons(_port);
	servaddr.sin_addr.s_addr = inet_addr("0.0.0.0");
	int enable = 1;
	setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));

	if (bind(sock_fd, (sockaddr*)&servaddr, sizeof(servaddr)) < 0)
		BindError();
	
	if (listen(sock_fd, SOMAXCONN) < 0)
		ListenError();
		
	struct	sockaddr_in	clientaddr;
	socklen_t			addrlen;
	char				buff[MAXLINE];
	int					user_fd;

	std::cout << "before accept" << std::endl;
	if ((user_fd = accept(sock_fd, (sockaddr *)&clientaddr, &addrlen)) < 0)
		AcceptError();
	std::cout << "running" << std::endl;
	while (true) {
		int readBytes = recv(user_fd, buff, 4096, 0);
		if (readBytes == 0)
			exit(-1);
		buff[readBytes] = '\0';
		printf("|%s|\n", buff);
		memset(buff, 0, MAXLINE);
	}
}
