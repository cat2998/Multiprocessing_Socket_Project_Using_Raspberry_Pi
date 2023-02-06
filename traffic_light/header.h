#ifndef HEADER_H
# define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <wiringPi.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>

#define BUFSIZE 30
#define n 5 //client count

#define trigPin 28
#define echoPin 29
#define RED 7
#define GREEN 21

int Distance();
void Red_On();
void Green_On();
void z_handler(int sig);
void error_handling(char *message);

#endif