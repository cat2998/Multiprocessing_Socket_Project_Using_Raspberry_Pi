#include "header.h"

int main(int argc, char *argv[])
{
   int x;
   struct sockaddr_in p;
   struct hostent *host;   // DNS processing ---
   char message[100];
   char buf[100];        // Dotted decimal
   char buffer[BUFSIZ];

   if ((x = socket(AF_INET, SOCK_STREAM, 0)) == -1)
   {
        perror("client: socket");
        return 1;
   }

   host = gethostbyname(argv[1]);
   if( host == NULL)
   {
       printf("Host not found !!: %s\n\r", argv[1]);
       return;
   }

   bzero((char *)&p, sizeof(p));
   p.sin_family = AF_INET;
   memcpy((char *)&p.sin_addr, host->h_addr, host->h_length);
   p.sin_port = htons(atoi(argv[2]));

   inet_ntop(AF_INET, &p.sin_addr, buf, sizeof(buf));
   printf("Trying %s ...\n", buf);
   if (connect(x, (struct sockaddr *)&p, sizeof(p)) == -1)
   {
        perror("client: connect");
        return 1;
   }
   printf("Connected %s...\n", argv[1]);

   if (wiringPiSetup() == -1)
   {
      return 1;
   }

   int distance = 0, i;
	long startTime, travelTime;
	if(wiringPiSetup () == -1)
		return 1;

	pinMode(trigPin, OUTPUT);
	pinMode(echoPin, INPUT);
	pinMode(RED, OUTPUT);
	pinMode(GREEN, OUTPUT);
	pinMode(BLUE, OUTPUT);
	digitalWrite(RED, 0);
	digitalWrite(GREEN, 0);
	digitalWrite(BLUE, 0);

	while(1) {
		digitalWrite(trigPin, LOW);
		usleep(2);
		digitalWrite(trigPin, HIGH);
		usleep(20);
		digitalWrite(trigPin, LOW);

		while(digitalRead(echoPin) == LOW);

		startTime = micros();
		while(digitalRead(echoPin) == HIGH);
		travelTime = micros() - startTime;

		distance = travelTime *17/1000;
		printf("Distance: %d cm\n", distance);

		if (0 < distance & distance < 10) {
			strcpy(message, "RED On");
   			printf("%s\n", message);
   			if (write(x, message, strlen(message) + 1) == -1)
   			{
				perror("client: write");
    			    	return 1;
   			}
			digitalWrite(RED, 1);
			digitalWrite(GREEN, 0);
			digitalWrite(BLUE, 0);
		}

		else if (10 <= distance & distance <= 20) {
			strcpy(message, "BLUE On");
    		  	printf("%s\n", message);
   			if (write(x, message, strlen(message) + 1) == -1)
    			{
    		    	 	perror("client: write");
    		    		return 1;
    		  	}
			digitalWrite(RED, 0);
			digitalWrite(BLUE, 1);
			digitalWrite(GREEN, 0);
		}

		else {
			strcpy(message, "GREEN On");
      			printf("%s\n", message);
      			if (write(x, message, strlen(message) + 1) == -1)
      			{
        			perror("client: write");
        			return 1;
      			}
			digitalWrite(GREEN, 1);
			digitalWrite(RED, 0);
			digitalWrite(BLUE, 0);
		}
      	delay(1000);
	}

   close(x);
   return 0;
}
