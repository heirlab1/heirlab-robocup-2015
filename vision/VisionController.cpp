

/* 
* VisionController.cpp
 *
 *  Created on: Jul 1, 2015
 *      Author: ryan & mo
*/

#include "VisionController.h"


// Returns distance of the ball from the between the feet in METERS
float VisionController::getBallDistance() {
	return tracking.getBallParameters().distance;
}

// Returns the angle of the ball if on screen in DEGREES
// 0 is straight ahead, negative is to the left, positive is to the right
float VisionController::getBallAngle() {
	return tracking.getBallParameters().angle;
}

// Returns last time the ball was seen in SECONDS
float VisionController::getBallLastSeen() {
	return tracking.getBallParameters().lastSeen;
}

int VisionController::startThread( ) {
	printf("started test thread");
	return 1;
}

int VisionController::killThread() {
	printf("killed fake thread");
	return 1;
}

int VisionController::submitTask(int command){
	printf("Received command: %d\n",command);
}



VisionController::VisionController() {
	// TODO Auto-generated constructor stub
	printf("Constructed VisionController Object");
}

VisionController::~VisionController() {
	// TODO Auto-generated destructor stub
}
