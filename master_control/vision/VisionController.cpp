

/* 
* VisionController.cpp
 *
 *  Created on: Jul 1, 2015
 *      Author: ryan & mo
*/

#include <stdio.h>
#include "VisionController.h"


// Returns distance of the ball from the FOOT in METERS
float VisionController::getBallDistance(){
	return 2.0;
}

// Returns the angle of the ball if on screen in DEGREES
// 0 is straight ahead, negative is to the left, positive is to the right
float VisionController::getBallAngle(){
	return -20.0;
}

// Returns last time the ball was seen in SECONDS
float VisionController::getBallLastSeen(){
	return 3.0;
}


int VisionController::startThread(){
	printf("started test thread");
	return 1;
}

int VisionController::killThread(){
	printf("killed fake thread");
	return 1;
}

int VisionController::submitTask(int command){
	printf("Recieved command: %d\n",command);
	return 1;
}



VisionController::VisionController() {
	// TODO Auto-generated constructor stub
	printf("Constructed VisionController Object\n");
}

VisionController::~VisionController() {
	// TODO Auto-generated destructor stub
}