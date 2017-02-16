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
	printf("Initalizing GPIO...\n\n");
	initGPIO();  // Setup GPIO and motorControl. Must be called before use!	
	while(!motorReady()){}; // Wait for the thread to be ready.
	
	// * * * * * * * * * * * * Timing test * * * * * * * * * * * * * * * * * * *
	
	// Forward
	printf("Calling move forward at speed 5...\n");
	clock_t strt = clock(), diff;	// Current CPU time
	moveForward(5);					// Tell CanSat to move forward at spd 5
	diff = (clock() - strt);		// Time since function call
	float msec = diff, msec2;		// convert to float
	while(!motorReady()){}			// Wait for thread to finish
	diff = (clock() - strt);		// Time since function call
	msec2 = diff/1000.0;			// Convert to millisecons.
	printf("Function call Time:  %f microseconds.\n",msec);
	printf("Motor Command Time:  %f milliseconds.\n",msec2);
	
	// Backward
	printf("\nCalling move backward at speed 4...\n");
	strt = clock();				// Current CPU time
	moveBackward(10);			// Tell CanSat to move backward
	diff = (clock() - strt);	// Time that occured during call
	msec = diff;				// Microseconds
	while(!motorReady()){}		// Wait for thread to finish
	diff = (clock() - strt);	// Time it took for thread to finish
	msec2 = diff/1000.0;		// Convert to milliseconds.
	printf("Function call Time:  %f microseconds.\n",msec);
	printf("Motor Command Time:  %f milliseconds.\n",msec2);	
	
	// Halt
	printf("\nCalling halt...\n");
	strt = clock();				// Current CPU time.
	halt();						// Tell CanSat to stop.
	diff = (clock() - strt);	// Time that occured during call
	msec = diff;				// Microseconds
	while(!motorReady()){}		// Wait for thread to finish
	diff = (clock() - strt);	// Time that occured since call
	msec2 = diff/1000.0;		// Convert to milliseconds.
	printf("Function call Time:  %f microseconds.\n",msec);
	printf("Motor Command Time:  %f milliseconds.\n",msec2);
	
	// Turn Right
	printf("\nCalling turn right hard at angle 90...\n");
	strt = clock();				// Current CPU time.
	turnRightHard(90);			// Tell CanSat to turn right hard.
	diff = (clock() - strt);	// Time that occured during call
	msec = diff;				// Microseconds
	while(!motorReady()){}		// Wait for thread to finish
	diff = (clock() - strt);	// Time that occured since call
	msec2 = diff/1000.0;		// Convert to milliseconds.
	printf("Function call Time:  %f microseconds.\n",msec);
	printf("Action completed (dependent on angle, speed):  %f milliseconds.\n",msec2);
	
	// Turn Right Soft 90 degrees.
	printf("\nCalling turn right Soft at angle 90...\n");
	strt = clock();				// Current CPU time.
	turnRightSoft(90);			// Tell CanSat to turn right.
	diff = (clock() - strt);	// Time that occured during call
	msec = diff;				// Microseconds
	while(!motorReady()){}		// Wait for thread to finish
	diff = (clock() - strt);	// Time that occured since call
	msec2 = diff/1000.0;		// Convert to milliseconds.
	printf("Function call Time:  %f microseconds.\n",msec);
	printf("Action completed (dependent on angle, speed):  %f milliseconds.\n",msec2);
	printf("Press Enter to run motor forward at all speeds.");
	getchar();
	int x = 0;
	while(x < 11) {
		printf("\nMoving forward at speed %d\n",x);
		moveForward(x);
		delay(3000);
		x++;
	}
	halt();
	printf("Press Enter to run motor backward at all speeds.");
	getchar();
	x = 0;
	while(x < 11) {
		printf("\nMoving backward at speed %d\n",x);
		moveBackward(x);
		delay(3000);
		x++;
	}
	printf("Testing complete!\n");
	return 0;
}//main
