/*
 * Tracking.h
 *
 *  Created on: Jun 29, 2015
 *      Author: ryan
 */

#ifndef TRACKING_H_
#define TRACKING_H_

#include "Vision.h"
#include "Head.h"


struct ballPhysicalParameters {
	std::clock_t lastSeen;
	float distance;
	float angle;
};

struct goalPhysicalParameters {
	std::clock_t lastSeen;
	bool found;
	int x, y;
};

class Tracking {
private:
	pthread_mutex_t ballPhysicalParametersLock;
	pthread_mutex_t goalPhysicalParametersLock;

	ballPhysicalParameters ball;

	Head head;
	Vision vision;

public:
	void centerBall(void);

	ballPhysicalParameters getBallPhysicalParameters(void);
	void setBallPhysicalParameters(ballPhysicalParameters);

	Tracking();
	virtual ~Tracking();
};

#endif /* TRACKING_H_ */
