

/* 
* vision_controller.h
 *
 *  Created on: Jul 1, 2015
 *      Author: ryan & mo
*/

#ifndef VISIONCONTROLLER_H_
#define VISIONCONTROLLER_H_

#include "pthread.h"
#include <ctime>
#include <stdio.h>


struct ballParameters {
	std::clock_t lastSeen;
	float distance;
	float angle;
};



struct screenParameters {
	bool found;
	int x, y, radius;
};

class VisionController {
public:
	VisionController();
	virtual ~VisionController();

	float getBallDistance();
	float getBallAngle();
	float getBallLastSeen();

	int startThread();
	int killThread();

	int submitTask(int command);

};

#endif