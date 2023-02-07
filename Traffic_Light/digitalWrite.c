#include "header.h"

void Red_On() {
	digitalWrite(RED, 1);
	digitalWrite(GREEN, 0);
}

void Green_On() {
	digitalWrite(GREEN, 1);
	digitalWrite(RED, 0);
}