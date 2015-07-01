/*

 * BallObject.h
 *
 *  Created on: Jul 1, 2015
 *      Author: ryan


#ifndef BALLOBJECT_H_
#define BALLOBJECT_H_

#include "pthread.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <ctime>

struct physicalParameters {
	std::clock_t lastSeen;
	float distance;
	float angle;
};

struct screenParameters {
	bool found;
	int x, y, radius;
};

class BallObject {
public:
	BallObject();
	virtual ~BallObject();
};

#endif  BALLOBJECT_H_
*/
