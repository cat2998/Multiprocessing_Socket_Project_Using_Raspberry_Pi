#include "header.h"

int main(int argc, char **argv)
{
    int sock;
    pid_t pid;
    int i;
    int distances[1] = { 0.0 };
    
    char message[BUFSIZE];
    int str_len, recv_len, recv_num;
    

    struct sockaddr_in serv_addr;

    if (argc != 3) {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }
    if (wiringPiSetup() == -1)
        return -1;

    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(RED, OUTPUT);
    pinMode(GREEN, OUTPUT);
    digitalWrite(RED, 1);
    digitalWrite(GREEN, 0);

    sock = socket(PF_INET, SOCK_STREAM, 0);

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error!");

    while (1) {
        read(sock, message, sizeof(message));
        printf("from server : %s\n", message);

        if (strcmp(message, "Red on!") == 0) {
            Red_On();
        }
        else if (strcmp(message, "Green on!") == 0) {
            Green_On();
        }
    }
    
    close(sock);
    return 0;
}

