#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <pthread.h>
#include <time.h>
#include <softPwm.h>

int curSpeed     = 0;	// Indicates current speed
int curAction    = 0;	// Indicates current action
int action       = 0;	// Communication to thread
int threadReady  = 0;	// Communication indicating thread is busy
int angleSpeed_i = 0;	// Communication to thread

#define rightMotorDir   16 // Pin for right motor direction  WHITE
#define rightMotorSpeed 21 // Pin for right motor speed      BLACK
#define leftMotorDir    26 // Pin for left motor direction   PURPLE
#define leftMotorSpeed  20 // Pin for left motor speed		 BLUE

#define FORWARD  0  // Unsure if active high or low so easily changed here
#define BACKWARD 1  // Likewise

/*******************************************************************************
/ void moveForward
/ This functios takes in a speed, and then translates it into forward motion
/ On the CanSat. The integer speed will be from 1-10. This func is non-blocking
*******************************************************************************/
void moveForward(int speed){
	angleSpeed_i = speed;	// Give thread the desired speed.
	action = 1;				// Alert thread to move forward.
	threadReady = 0;		// Make sure no other functions are called.
	return;
}//moveFoward

/*******************************************************************************
/ void moveForwardBlocking
/ This functios takes in a speed, and then translates it into forward motion
/ On the CanSat. The integer speed will be from 1-10. This func is blocking
*******************************************************************************/
void moveForwardBlocking(int speed) {
	digitalWrite(rightMotorDir, FORWARD); // Make direction Right forward
	digitalWrite(leftMotorDir,  FORWARD); // Make direction Left forward
	softPwmWrite(rightMotorSpeed, speed*10); // Make speed the desired speed
	softPwmWrite(leftMotorSpeed,  speed*10); // Make speed the desired speed
	curAction = 1; 		// We are now moving foward 
	curSpeed  = speed;  // At this speed.
}//moveForwardBlocking

/*******************************************************************************
/ void moveBackward
/ This functions takes in a speed, and then translates it into backward motion
/ On the CanSat. The integer speed will be from 1-10.
*******************************************************************************/
void moveBackward(int speed){
	angleSpeed_i = speed;	// Give thread the desired speed.
	action = 2;				// Alert thread to move backward.
	threadReady = 0;		// Make sure no other functions are called.
	return;
}//moveBackward

/*******************************************************************************
/ void moveBackwardBlocking
/ This functions takes in a speed, and then translates it into backward motion
/ On the CanSat. The integer speed will be from 1-10. This func is blocking
*******************************************************************************/
void moveBackwardBlocking(int speed) {
	digitalWrite(rightMotorDir, BACKWARD);	// Make right motor go backward
	digitalWrite(leftMotorDir,  BACKWARD);  // Make left motor go backward
	softPwmWrite(rightMotorSpeed, speed*10); // Update Speed
	softPwmWrite(leftMotorSpeed,  speed*10); // Update Speed
	curAction = 2;    	// Update current action
	curSpeed = speed;	// Update current speed
}//MoveBackwardBlocking

/*******************************************************************************
/ void halt
/ This functions makes the CanSat stop moving.
*******************************************************************************/
void halt() {
	action = 3;			// Alert thread to stop CanSat
	threadReady = 0;	// Make sure no other functions are called.
	return;
}//halt

/*******************************************************************************
/ void haltBlocking
/ This functions makes the CanSat stop moving. This func is blocking.
*******************************************************************************/
void haltBlocking() {
	softPwmWrite(leftMotorSpeed,  0);		// Update Speed to 0
	softPwmWrite(rightMotorSpeed, 0);		// Update Speed to 0
	digitalWrite(leftMotorDir, FORWARD);	// Default motor dir is fwd
	digitalWrite(rightMotorDir, FORWARD);	// Default motor dir is fwd
	curAction  = 3;		// Update current action.
	curSpeed = 0;		// Update current speed.
}//haltBlocking

/*******************************************************************************
/ void turnRight
/ This functions takes in an angle, and then translates it into a right turn
/ at that angle. The integer angle will be from 0-360. Any angle greater than
/ 45 degrees will require a full stop before turning. The CanSat will then
/ continue doing whatever it was doing before. This func is non-Blocking
*******************************************************************************/
void turnRightHard(int angle){
	angleSpeed_i = angle;	// Give thread the angle to turn.
	action = 4;				// Alert thread to turn.
	threadReady = 0;		// Make sure no other functions are called.
	return;
}//turnRight

/*******************************************************************************
/ void turnRightSoft
/ This functions takes in an angle, and then translates it into a right turn
/ at that angle. The integer angle will be from 0-360. Any angle greater than
/ 45 degrees will require a full stop before turning. The CanSat will then
/ continue doing whatever it was doing before. This func is non-Blocking
*******************************************************************************/
void turnRightSoft(int angle){
	angleSpeed_i = angle;	// Give thread the angle to turn.
	action = 41;				// Alert thread to turn.
	threadReady = 0;		// Make sure no other functions are called.
	return;
}//turnRight

/*******************************************************************************
/ void turnRightBlocking
/ This functions takes in an angle, and then translates it into a right turn
/ at that angle. The integer angle will be from 0-360. Any angle greater than
/ 45 degrees will require a full stop before turning. The CanSat will then
/ continue doing whatever it was doing before. This function is blocking 
*******************************************************************************/
void turnRightSoftBlocking(int angle) {
	
	int prevAction = curAction; // What we were doing before

	if(curAction == 3) { // If we were stopped.
		curSpeed = 5;    // Default speed. Default direction is forward.
	}//if curAction
	
	curAction = 4;

	softPwmWrite(leftMotorSpeed,  curSpeed*10); // Ensure left is moving
	softPwmWrite(rightMotorSpeed, curSpeed*5);	// reduce right speed
	delay(angle*60/((curSpeed/3)+1));			// Math will need fixed
	softPwmWrite(rightMotorSpeed, curSpeed*10); // Return to going forward

	curAction = prevAction; // Reset current action
	
	if(prevAction == 3) { // We were stopped.
		haltBlocking();   // Now we aren't.
	}
	
}//turnRightSoftBlocking

/*******************************************************************************
/ void turnRightHardBlocking
/ This functions takes in an angle, and then translates it into a right turn
/ at that angle. The integer angle will be from 0-360. Any angle greater than
/ 45 degrees will require a full stop before turning. The CanSat will then
/ continue doing whatever it was doing before. This function is blocking 
*******************************************************************************/
void turnRightHardBlocking(int angle) {
	
	int prevAction = curAction; // What we were doing before

	if(curAction == 3) { // if we were stopped.
		curSpeed = 5;    // default speed. Default direction is forward
	}//if curAction
	
	int prevSpeed = curSpeed;
	curAction = 4;

	haltBlocking(); // Stop the CanSat
	curAction = 4;  // Reset our current action
	curSpeed = prevSpeed;
	softPwmWrite(rightMotorSpeed, curSpeed*10);	// ensure we're moving
	softPwmWrite(leftMotorSpeed,  curSpeed*10); // ensure we're moving
	digitalWrite(rightMotorDir, BACKWARD); // Backup right motor
	digitalWrite(leftMotorDir,  FORWARD);  // Move left motor forward.
	delay((angle*15)/(curSpeed/3+1));      // Math will need checked
		
	if (prevAction == 1) { // if we were moving forward
		digitalWrite(rightMotorDir, FORWARD); // Move foward again
		curAction = 1; 	// Reset current action
	} //if prevAction
	else if (prevAction == 2) {  // If we were backing up 
		digitalWrite(leftMotorDir, BACKWARD); // Back up again
		curAction = 2; 	// Reset current action.
	} //else if prevAction

	curAction = prevAction; // Reset current action
	
	if(prevAction == 3) { // We were stopped.
		haltBlocking();   // Now we aren't.
	}
	
}//turnRightHardBlocking

/*******************************************************************************
/ void turnLeft
/ This functions takes in an angle, and then translates it into a left turn
/ at that angle. The integer angle will be from 0-180. Any angle greater than
/ 45 degrees will require a full stop before turning.
*******************************************************************************/
void turnLeftHard(int angle){
	angleSpeed_i = angle;	// Give thread the angle to turn.
	action = 5;				// Alert thread to turn left.
	threadReady = 0;		// Make sure no other functions are called.
	return;
}//turnLeftHard

/*******************************************************************************
/ void turnLeft
/ This functions takes in an angle, and then translates it into a left turn
/ at that angle. The integer angle will be from 0-180. Any angle greater than
/ 45 degrees will require a full stop before turning.
*******************************************************************************/
void turnLeftSoft(int angle){
	angleSpeed_i = angle;	// Give thread the angle to turn.
	action = 51;				// Alert thread to turn left.
	threadReady = 0;		// Make sure no other functions are called.
	return;
}//turnLeftSoft

/*******************************************************************************
/ void turnLeftHardBlocking
/ This functions takes in an angle, and then translates it into a right turn
/ at that angle. The integer angle will be from 0-360. Any angle greater than
/ 45 degrees will require a full stop before turning. The CanSat will then
/ continue doing whatever it was doing before. This function is blocking 
*******************************************************************************/
void turnLeftHardBlocking(int angle) {
	
	int prevAction = curAction; // What we were doing before

	if(curAction == 3) { // if we were stopped.
		curSpeed = 5;    // default speed. Default direction is forward
	}//if curAction
	
	int prevSpeed = curSpeed;

	curAction = 4;

	haltBlocking(); // Stop the CanSat
	curAction = 4;  // Reset our current action
	curSpeed  = prevSpeed;
	softPwmWrite(rightMotorSpeed, curSpeed*10);
	softPwmWrite(leftMotorSpeed,  curSpeed*10);
	digitalWrite(rightMotorDir, FORWARD); 	// Make right motor forward
	digitalWrite(leftMotorDir,  BACKWARD);  // Move left motor backward.
	delay((angle*15)/(curSpeed/3+1));      	// Math will need checked
	
	if (prevAction == 1) { // if we were moving forward
		digitalWrite(leftMotorDir, FORWARD); // Move foward again
		curAction = 1; 	// Reset current action
	} //if prevAction
	else if (prevAction == 2) {  // If we were backing up 
		digitalWrite(rightMotorDir, BACKWARD); // Back up again
		curAction = 2; 	// Reset current action.
	} //else if prevAction

	curAction = prevAction; // Reset current action
	
	if(prevAction == 3) { // We were stopped.
		haltBlocking();   // Now we aren't.
	}	
}//turnLeftHardBlocking

/*******************************************************************************
/ void turnLeftSoftBlocking
/ This functions takes in an angle, and then translates it into a right turn
/ at that angle. The integer angle will be from 0-360. Any angle greater than
/ 45 degrees will require a full stop before turning. The CanSat will then
/ continue doing whatever it was doing before. This function is blocking 
*******************************************************************************/
void turnLeftSoftBlocking(int angle) {
	
	int prevAction = curAction; // What we were doing before

	if(curAction == 3) { // if we were stopped.
		curSpeed = 5;    // default speed. Default direction is forward
	}//if curAction
	
	curAction = 4;

	softPwmWrite(rightMotorSpeed,  curSpeed*10);	// Make sure moving
	softPwmWrite(leftMotorSpeed, curSpeed*5);	// reduce leftspeed
	delay(angle*60/((curSpeed/3)+1));	// Math will need fixed
	softPwmWrite(leftMotorSpeed, curSpeed*10); // Return to going forward

	curAction = prevAction; // Reset current action
	
	if(prevAction == 3) { // We were stopped.
		haltBlocking();   // Now we aren't.
	}
	
}//turnLeftBlocking

/***************************************************************************** 
/ motorReady
/ Returns 0 if the thread is doing an operation, i.e. in the process of 
/ turning. Returns a 1 if the motor is ready to accept commands.
*****************************************************************************/
int motorReady() {
	return threadReady; // threadReady is one when the thread is ready.
}

/*****************************************************************************
/ motorAction
/ Returns the current motor action (1) means moving forward, (2) means moving
/ backward, (3) means halting, (4) means turning right, (5) means turning 
/ left.
*****************************************************************************/
int motorAction() {
	return curAction;
}

/*****************************************************************************
/ motorSpeed
/ Returns the current speed that the motors are moving.
*****************************************************************************/
int motorSpeed() {
	return curSpeed;
}

/*****************************************************************************
/ motorThread
/ This is the thread that controls the motors. It is started when you call
/ initGPIO.
*****************************************************************************/
PI_THREAD (motorThread) {
	
	threadReady = 1;	// The thread has started.
	while(1) {					
		if (action == 1) { // Move forward
			moveForwardBlocking(angleSpeed_i); // Move forward
			action = 0;				 // Reset the action 
			threadReady = 1;		 // Thread is ready to accept more commands.
		}// Move foward
		else if (action == 2) {	//move backward
			moveBackwardBlocking(angleSpeed_i); // Move backward 
			action = 0;			// Reset the action
			threadReady = 1;	// The thread is ready to accept more commands.
		}// move backward
		else if (action == 3) { //halt
			haltBlocking(); // Stop fam.
			action = 0; 		// Reset the action
			threadReady = 1;    // Thread is ready to accept commands.
		}//halt
		else if (action == 4) { //turn right hard
			turnRightHardBlocking(angleSpeed_i);	// Turn right
			action = 0;			// Reset the action
			threadReady = 1; 	// Thread is ready to accept commands
		}//turn right
		else if (action == 41) { //turn right softly
			turnRightSoftBlocking(angleSpeed_i);
			action = 0;			// Reset the action
			threadReady = 1;	// Thread is ready again.
		}//turn right soft
		else if (action == 5) { //turn left
			turnLeftHardBlocking(angleSpeed_i); 	// Turn left
			action = 0;
			threadReady = 1;
		}//turn left
		else if (action == 51) {
			turnLeftSoftBlocking(angleSpeed_i);
			action = 0;
			threadReady = 1;
		}
	}//while
}//thread

/*******************************************************************************
/ void initGPIO
/ Thus far, this function only calls wiringPiSetupGpio in order to initialize
/ the necessary GPIO logic, and also starts the motor controlling thread. It 
/ also ensures the CanSat is not moving.
*******************************************************************************/
void initGPIO(void) {
	//wiringPiSetup();		// Simplified version
	wiringPiSetupGpio();	// Broadcom versioni
	piThreadCreate(motorThread); // Start the motor controlling thread.
	
	// Setup GPIO pins
	pinMode(rightMotorDir, OUTPUT);	// init right motor direction
	pinMode(leftMotorDir,  OUTPUT);	// init left motor direction
	softPwmCreate(leftMotorSpeed,  0, 100); // init left motor speed
	softPwmCreate(rightMotorSpeed,  0, 100); // intit right motor speed
	
	// Make sure we are stopped
	haltBlocking();
}

