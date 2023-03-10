#include "header.h"

int main(int argc, char **argv)
{
	int serv_sock;
	int clnt_sock;
	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	struct sigaction act;
	int addr_size, str_len, state;
	pid_t pid;
	int fd1[2], fd2[2];
	int distance[1] = {0};
	double second = 0; 
	double speed[1] = {0.0};
	double stop = 0;
	char message[BUFSIZE] = "";
	clock_t start, end;
	int count = 0, flag = 0; //client count

	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	act.sa_handler = z_handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	state = sigaction(SIGCHLD, &act, 0);                              /* 시그널 핸들러 등록 */
	if (state != 0) {
		puts("sigaction() error");
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	if (bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)
		error_handling("bind() error");

	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error");

	if ( pipe(fd1) == -1 || pipe(fd2) == -1)
		error_handling("pipe() error");

	pid=fork();

	if(pid==0) {

        while (1) {
			read(fd1[0], speed, sizeof(speed));
			write(fd2[1], speed, sizeof(speed));
		}
		return 0;
    }

	while (1) {
		addr_size = sizeof(clnt_addr);
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &addr_size);

		if (clnt_sock == -1)
			continue;
		count++;

		if ((pid = fork()) == -1) {
			close(clnt_sock);
			continue;
		}

		if (pid == 0) {     /* 자식 프로세스의 경우 */
		
			close(serv_sock);

			if (count == 1) {
				while (1) {
					read(clnt_sock, distance, BUFSIZE);
						
					if(distance[0] <= 70 && distance[0] > 20 && flag == 0) {
						start = clock();
						flag = 1;
						strcpy(message, "Red on!");
						write(clnt_sock, message, sizeof(message));
					}
					else if(5 < distance[0] && distance[0] <= 20 && flag == 1) {
						end = clock();
						flag = 2;
						second = (end-start) / (double)1000;
						printf("second = %.2f\n", second);
						strcpy(message, "Green on!");
						write(clnt_sock, message, sizeof(message));

						speed[0] = 50 / second;
						printf("speed = %.2f\n", speed[0]);
						write(fd1[1], speed, sizeof(speed));
					
						stop = 40 / speed;
						printf("stop = %.2f\n", stop);
						sleep(stop);
						strcpy(message, "Red on!");
						write(clnt_sock, message, sizeof(message));
						flag = 3;
					}
					else if(distance[0] <= 5) {
						flag = 0;
					}
				}
			}
			else {
				while (1) {
					read(fd2[0], speed, sizeof(speed));
					int s = (int)speed[0];
					itoa(s, message, 10);
					write(clnt_sock, message, sizeof(message));
					
					while (1) {
						read(clnt_sock, distance, BUFSIZE);
							
						if(distance[0] <= 70 && distance[0] > 20 && flag == 0) {
							start = clock();
							flag = 1;
						}
						else if(5 < distance[0] && distance[0] <= 20 && flag == 1) {
							end = clock();
							flag = 2;
							second = (end-start) / (double)1000;
							printf("second = %.2f\n", second);
							speed[0] = 50 / second;
							printf("speed = %.2f\n", speed[0]);
							write(fd1[1], speed, sizeof(speed));
							flag = 3;
						}
						else if(distance[0] <= 5) {
							flag = 0;
						}
					}
				}
			}
			return 0;
		}
	}

	close(serv_sock);
	return 0;
}
