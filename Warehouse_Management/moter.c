#include "header.h"

void Motor_CW()
{
	int i;

	for (i = 0; i < 5; i++) {
		digitalWrite(DC_INA, 0);
		digitalWrite(DC_INB, 1);
		sleep(1);
	}
	EN_count = 0;
}

void Step_motor()
{
	int i;
	for (i = 0; i < 512; i++) {
		digitalWrite(STEP_OUT2B, 1);
		digitalWrite(STEP_OUT2A, 0);
		digitalWrite(STEP_OUT1B, 0);
		digitalWrite(STEP_OUT1A, 0);
		usleep(3000);

		digitalWrite(STEP_OUT2B, 0);
		digitalWrite(STEP_OUT2A, 1);
		digitalWrite(STEP_OUT1B, 0);
		digitalWrite(STEP_OUT1A, 0);
		usleep(3000);

		digitalWrite(STEP_OUT2B, 0);
		digitalWrite(STEP_OUT2A, 0);
		digitalWrite(STEP_OUT1B, 1);
		digitalWrite(STEP_OUT1A, 0);
		usleep(3000);

		digitalWrite(STEP_OUT2B, 0);
		digitalWrite(STEP_OUT2A, 0);
		digitalWrite(STEP_OUT1B, 0);
		digitalWrite(STEP_OUT1A, 1);
		usleep(3000);
	}
}

void Nothing()
{
    digitalWrite(DC_INA, 0);
	digitalWrite(DC_INB, 0);
	digitalWrite(STEP_OUT2B, 0);
	digitalWrite(STEP_OUT2A, 0);
	digitalWrite(STEP_OUT1B, 0);
	digitalWrite(STEP_OUT1A, 0);
}
