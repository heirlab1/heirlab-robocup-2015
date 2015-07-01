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


struct ballParameters {
	std::clock_t lastSeen;
	float distance;
	float angle;
};

struct goalParameters {
	std::clock_t lastSeen;
	bool found;
	int x, y;
};

class Tracking {
private:
	Head head;
	Vision vision;

public:
	void centerBall(void);

	Tracking();
	virtual ~Tracking();
};

#endif /* TRACKING_H_ */
