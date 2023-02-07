#include "header.h"

int main(int argc, char **argv)
{
	int sock;
	pid_t pid;
	double temperature[1];
	char message[BUFSIZE];
	int str_len, recv_len, recv_num;

	struct sockaddr_in serv_addr;

	if (argc != 3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	if (wiringPiSetup() == -1)
			return -1;

	pinMode(DC_INA, OUTPUT);
	pinMode(DC_INB, OUTPUT);
	pinMode(DC_ENCA, OUTPUT);
	pinMode(DC_ENCB, OUTPUT);
	digitalWrite(DC_INA, 0);
	digitalWrite(DC_INB, 0);
	pullUpDnControl(DC_ENCA, PUD_UP);
	pullUpDnControl(DC_ENCB, PUD_UP);
	wiringPiISR(DC_ENCA, INT_EDGE_BOTH, &ENCA_interrupt);
	wiringPiISR(DC_ENCB, INT_EDGE_BOTH, &ENCB_interrupt);

	pinMode(STEP_OUT1A, OUTPUT);
	pinMode(STEP_OUT1B, OUTPUT);
	pinMode(STEP_OUT2A, OUTPUT);
	pinMode(STEP_OUT2B, OUTPUT);
	digitalWrite(STEP_OUT1A, 0);
	digitalWrite(STEP_OUT1B, 0);
	digitalWrite(STEP_OUT2A, 0);
	digitalWrite(STEP_OUT2B, 0);
	
	sock = socket(PF_INET, SOCK_STREAM, 0);

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("connect() error!");

	if ((pid = fork()) == -1) {
		close(sock);
	}
	
	if (pid == 0) 
	{
		while (1) {
			temperature[0] = dht11_read_val();
			write(sock, temperature, sizeof(temperature));
			read(sock, message, sizeof(message));
			printf("from server : %s\n", message);

			if(strcmp(message, "hitter on!") == 0) {
				Motor_CW();
			}
			else if(strcmp(message, "nothing!") == 0) {
				Nothing();
				sleep(5);
			}
			else if(strcmp(message, "aircondition on!") == 0) {
				Step_motor();
			}
		}
	}
	
	Nothing();
	close(sock);
	return 0;
}
