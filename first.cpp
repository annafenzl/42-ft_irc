
# include "server.hpp"

void	error_and_die(const char *error)
{
	std::cerr << error << std::endl;
	exit(-1);
}

int main(int argc, char **argv)
{
	if (argc < 2)
		error_and_die("program should be called with <port> <password");
	
	int		sock_fd, n;
	int		sendbytes;
	struct	sockaddr_in	servaddr;
	char	sendline[MAXLINE];
	char	recvline[MAXLINE];
	

	if ( (sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) // using TCP as default for stram sockets
		error_and_die("error while creating the socket!");

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET; //Address Family Internet
	servaddr.sin_port = htons(SERVER_PORT);
	servaddr.sin_addr.s_addr = inet_addr("0.0.0.0");
	int enable = 1;
	setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));

	if (bind(sock_fd, (sockaddr*)&servaddr, sizeof(servaddr)) < 0)
		error_and_die("bind failed!");
	
	if (listen(sock_fd, SOMAXCONN) < 0)
		error_and_die("listen failed!");
		
	struct	sockaddr_in	clientaddr;
	socklen_t			addrlen;
	char				buff[4096];
	int					user_fd;

	std::cout << "before accept" << std::endl;
	if ((user_fd = accept(sock_fd, (sockaddr *)&clientaddr, &addrlen)) < 0)
		error_and_die("accept failed!");
	std::cout << "running" << std::endl;
	while (true) {
		int readBytes = recv(user_fd, buff, 4096, 0);
		if (readBytes == 0)
			exit(-1);
		buff[readBytes] = '\0';
		printf("|%s|\n", buff);
		memset(buff, 0, MAXLINE);
	}
	
	return 0;
}