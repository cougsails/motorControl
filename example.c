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

	int x = 0;

	printf("Initalizing GPIO...\n\n");
	initGPIO();  // Setup GPIO and motorControl. Must be called before use!	
	while(!motorReady()){}; // Wait for the thread to be ready.
	
	// * * * * * * * * * * * * Timing test * * * * * * * * * * * * * * * * * * *
	
	// Forward
	printf("Calling move forward.\n\n");
	clock_t strt = clock(), diff;	// Current CPU time
	moveForward(5);					// Tell CanSat to move forward at spd 5
	diff = (clock() - strt);		// Time since function call
	float msec = diff, msec2;		// convert to float
	while(!motorReady()){}			// Wait for thread to finish
	diff = (clock() - strt);		// Time since function call
	msec2 = diff/1000.0;			// Convert to millisecons.
	printf("Function call Time:  %f microseconds.\n",msec);
	printf("Motor Command Time:  %f milliseconds.\n",msec2);

	while(x < 11) {
		printf("\nMoving forward at speed %d\n",x);
		moveForward(x);
		delay(1000);
		x++;
	}

	halt();

	printf("Press Enter to begin the Move Backwards Test...");
	getchar();

	// Backward
	printf("\nCalling move backward.\n\n");
	strt = clock();				// Current CPU time
	moveBackward(10);			// Tell CanSat to move backward
	diff = (clock() - strt);	// Time that occured during call
	msec = diff;				// Microseconds
	while(!motorReady()){}		// Wait for thread to finish
	diff = (clock() - strt);	// Time it took for thread to finish
	msec2 = diff/1000.0;		// Convert to milliseconds.
	printf("Function call Time:  %f microseconds.\n",msec);
	printf("Motor Command Time:  %f milliseconds.\n",msec2);	
	
	x = 0;
	while(x < 11) {
		printf("\nMoving backward at speed %d\n",x);
		moveBackward(x);
		delay(1000);
		x++;
	}

	halt();

	printf("Press Enter to turn right hard 20 degrees\n");
	getchar();
	turnRightHard(20);
	while(!motorReady()){}

	printf("Press Enter to turn right hard 45 degrees\n");
	getchar();
	turnRightHard(45);
	while(!motorReady()){}

	printf("Press Enter to turn right hard 90 degrees\n");
	getchar();
	turnRightHard(90);
	while(!motorReady()){}

	printf("Press Enter to turn right soft 20 degrees\n");
	getchar();
	turnRightSoft(20);
	while (!motorReady()){}

	printf("Press Enter to turn right soft 45 degrees\n");
	getchar();
	turnRightSoft(45);
	while(!motorReady()){}

	printf("Press Enter to turn right soft 90 degrees\n");
	getchar();
	turnRightSoft(90);
	while(!motorReady()){}

	printf("Press Enter to turn left hard 20 degrees\n");
	getchar();
	turnLeftHard(20);
	while(!motorReady()){}

	printf("Press Enter to turn left hard 45 degrees\n");
	getchar();
	turnLeftHard(45);
	while(!motorReady()){}

	printf("Press Enter to turn left hard 90 degrees\n");
	getchar();
	turnLeftHard(90);
	while(!motorReady()){}

	printf("Press Enter to turn left soft 20 degrees\n");
	getchar();
	turnLeftSoft(20);
	while (!motorReady()){}

	printf("Press Enter to turn left soft 45 degrees\n");
	getchar();
	turnLeftSoft(45);
	while(!motorReady()){}

	printf("Press Enter to turn left soft 90 degrees\n");
	getchar();
	turnLeftSoft(90);
	while(!motorReady()){}
	
	printf("Testing complete!\n");
	return 0;

}//main
