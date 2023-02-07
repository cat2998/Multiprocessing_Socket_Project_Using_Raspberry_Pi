#ifndef HEADER_H
# define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <wiringPi.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>

#define BUFSIZE 30
#define MAX_TIME 100
#define DHT11PIN 25

#define DC_INA 26
#define DC_INB 23
#define DC_ENCA 10
#define DC_ENCB 11

#define STEP_OUT1A 27
#define STEP_OUT1B 0
#define STEP_OUT2A 1
#define STEP_OUT2B 24

unsigned long int EN_count = 0;
char clockwise = 0;

void Motor_CW();
void Step_motor();
void Nothing();

void ENCA_interrupt();
void ENCB_interrupt();

double dht11_read_val();

void z_handler(int sig);
void error_handling(char *message);


#endif