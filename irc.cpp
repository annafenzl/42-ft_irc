/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afenzl <afenzl@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/12 17:06:05 by afenzl            #+#    #+#             */
/*   Updated: 2023/03/12 17:51:55 by afenzl           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "irc.hpp"

# define MAXLINE 4096

// using err_n_die>

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		perror("not the right amount of parameters provided!");
		return (-1);
	}
	
	int		sock_fd, n;
	int		sendbytes;
	struct	sockaddr_in	servaddr;
	char	sendline[MAXLINE];
	char	recvline[MAXLINE];
	

	if ( (sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) // using TCP as default for stram sockets
		perror("error while creating the socket!");

	bzero(&servaddr, sizeof(servaddr)); // maybe change this to memset, since bzero is deprecated
	servaddr.sin_family = AF_INET; //Address Family Internet
	servaddr.sin_port = htons(SERVER_PORT);
	servaddr.sin_addr.s_addr = inet_addr("0.0.0.0");
	int enable = 1;
	setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));

	if (bind(sock_fd, (sockaddr*)&servaddr, sizeof(servaddr)) < 0)
		perror("bind failed!");
	
	if (listen(sock_fd, SOMAXCONN) < 0)
		perror("listen failed!");
		
	struct	sockaddr_in	clientaddr;
	socklen_t			addrlen;
	char				buff[4096];
	int					user_fd;

	if ((user_fd = accept(sock_fd, (sockaddr *)&clientaddr, &addrlen)) < 0)
		perror("accept failed!");
	while (true) {
		int readBytes = recv(user_fd, buff, 4096, 0);
		if (readBytes == 0)
			exit(-1);
		buff[readBytes] = '\0';
		printf("|%s|\n", buff);
		memset(buff, 0, 4096);
	}
	
	return 0;
}