#include <stdio.h>
#include <motorControl.h>
#include <wiringPi.h>
#include <time.h>

//Two pins per motor; one for forward, one for backward.

/*******************************************************************************
/ int main
/ This is the main function and is called at startup. Operation of main will
/ be mentioned shortly.
*******************************************************************************/
int main() {
	initGPIO();  // Setup GPIO.
	int x = 0;
	while(!motorReady()){};
	while(1) {
		moveForward(8);
		delay(1000);
		turnRight(20);
		while(!motorReady()){}
	}
	delay(50000);
	clock_t diff;
	clock_t strt = clock();
	//do things
	diff = (clock() - strt);
	float msec = diff;
	printf("This went for %f microseconds.\n",msec);
	//printf("You are currently moving at a speed of: %d\n",motorSpeed());
	//moveBackward();
	//while(!motorReady()) {};
	
	//printf("You are currently moving: %d\n",motorAction());
	return 0;
}
