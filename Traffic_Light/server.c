#include "header.h"

int main(int argc, char **argv)
{
	int serv_sock;
	int clnt_sock[n];
	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	struct sigaction act;
	int addr_size, str_len, state;
	pid_t pid;
	int fd1[2];
	int distance[1] = {0};
	double second = 0; 
	double speed;
	double stop[1] = {0.0};
	double next[1] = {0.0};
	char message[BUFSIZE] = "";
	clock_t start, end;
	int count = 0, a = 0; //client count

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

	if ( pipe(fd1) == -1 )
		error_handling("pipe() error");

	while (1) {
		addr_size = sizeof(clnt_addr);
		clnt_sock[count] = accept(serv_sock, (struct sockaddr*)&clnt_addr, &addr_size);

		if (clnt_sock[count] == -1)
			continue;
		
		count++;

		if ((pid = fork()) == -1) {
			close(clnt_sock[count]);
			continue;
		}
		if (pid == 0) {     /* 자식 프로세스의 경우 */
		
			close(serv_sock);
			if(count==1) {
			if(clnt_sock[0]) {
				start = clock();
				while (1) {
					read(clnt_sock[0], distance, BUFSIZE);
					
					if(distance[0] <= 70 && distance[0] > 20) {
						strcpy(message, "Red on!");
						write(clnt_sock[0], message, sizeof(message));
					}
					else if(5 < distance[0] && distance[0] <= 20 && a == 0) {
						end = clock();
						a += 1;
						second = (end-start) / (double)1000;
						printf("second = %.2f\n", second);
						strcpy(message, "Green on!");
						write(clnt_sock[0], message, sizeof(message));

						speed = 50 / second;
						printf("speed = %.2f\n", speed);
					
						stop[0] = 40 / speed;
						next[0] = 100 / speed;
						printf("stop = %.2f\n", stop[0]);
						printf("next = %.2f\n\n", next[0]);
						write(fd1[1], stop, sizeof(stop));
						write(fd1[1], next, sizeof(next));
						write(fd1[1], next, sizeof(next));

						sleep(stop[0]);
						strcpy(message, "Red on!");
						write(clnt_sock[0], message, sizeof(message));
					}
					else if(distance[0] <= 5) {
						a = 0;
						start = clock();
						stop[0] = 0.0;
						next[0] = 0.0;
					}
				}
			}
			}
			return 0;
		}

		else  { //parent
            if(clnt_sock[1]) {
                while(1) {
                read(fd1[0], next, sizeof(next));
                read(fd1[0], stop, sizeof(next));
                read(fd1[0], next, sizeof(stop));

                printf("next : %.2f\n", next[0]);
                printf("stop : %.2f\n\n", stop[0]);
                for (int i = 1; i < n; i++)
                {
                    sleep(next[0]);
                    strcpy(message, "Green on!");
                    write(clnt_sock[i], message, sizeof(message));
                
                    sleep(stop[0]);
                    strcpy(message, "Red on!");
                    write(clnt_sock[i], message, sizeof(message));
                }
                }
            }
        }
	}

	close(serv_sock);
	return 0;
}
