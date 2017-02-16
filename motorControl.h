#include <pthread.h>

extern int curSpeed;
extern int curAction;
extern int action;
extern int threadReady;
extern int angleSpeed_i;

void moveForward(int speed);			
void moveForwardBlocking(int speed);	
void moveBackward(int speed);
void moveBackwardBlocking(int speed);
void halt();
void haltBlocking();
void turnRight(int angle);
void turnRightBlocking(int angle);
void turnLeft(int angle);
void turnLeftBlocking(int angle);
int  motorReady();
int  motorAction();
int  motorSpeed();
void initGPIO();
