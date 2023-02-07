#include "header.h"

int main(int argc, char *argv[])
{
	int x, y;
	int clilen;
	struct sockaddr_in addr, cli_addr;
	char buf[100000];
	char buffer[BUFSIZ];
	if (( x = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("server: socket");
		return 1;
	} 

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(atoi(argv[1]));
	if (bind(x, (struct sockaddr *)&addr, sizeof(addr)) == -1)
	{
		perror("server: bind");
		return 1;
	}

	if (listen(x, 5) == -1)
	{
		perror("server: listen");
		return 1;
	}

	printf("server start !!! \n");
	clilen = sizeof(cli_addr);

	if ((y = accept (x, (struct sockaddr *)&cli_addr, &clilen)) == -1) {
		perror("server: accept");
		return 1;
	}

	
	inet_ntop(AF_INET, &cli_addr.sin_addr, buf, sizeof(buf)); // client address
	printf("client address => %s\n", buf);

	while(1)
	{
		if (read(y, buffer, BUFSIZ) == -1)
	{
		perror("server: read");
		return 1;
	}
		printf("receive message: %s\n", buffer);
	}

	close(x);
	close(y);
	return 0;
}
