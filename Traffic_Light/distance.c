#include "header.h"

int Distance() {
	int distance = 0;
	long startTime, travelTime;
			digitalWrite(trigPin, LOW);
			usleep(2);
			digitalWrite(trigPin, HIGH);
			usleep(20);
			digitalWrite(trigPin, LOW);

			while (digitalRead(echoPin) == LOW);

			startTime = micros();
			while (digitalRead(echoPin) == HIGH);
			travelTime = micros() - startTime;

			distance = travelTime * 17 / 1000;
			if (distance <= 70) {
			printf("Distance: %d cm\n", distance);
			}
			return distance;
}