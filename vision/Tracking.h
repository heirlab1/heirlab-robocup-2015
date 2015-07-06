/*
 * Tracking.h
 *
 *  Created on: Jun 29, 2015
 *      Author: ryan
 */

#ifndef TRACKING_H_
#define TRACKING_H_

#include "Head.h"
#include "BallObject.h"




class Tracking {
private:
	pthread_mutex_t ballPhysicalParametersLock;
	pthread_mutex_t goalPhysicalParametersLock;

	Head head;
	BallObject* ball;

public:
	void setPointers(BallObject*);

	void centerBall(void);
	void searchBall(void);

	ballPhysicalParameters getBallPhysicalParameters(void);
	void setBallPhysicalParameters(ballPhysicalParameters);

	Tracking();
	virtual ~Tracking();
};

#endif /* TRACKING_H_ */
