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
#include <ctime>

//(1=100% seen, 0.5=50% seen)
#define searchThreshold 1 
 
//Area that will be marked as seen (Degrees)
#define searchRadius 60 

//Min time between dynamixel communication bursts (1 = 1s, 0.5 = 500ms)
#define maxTime 0.1 

struct BFSdata {
	std::clock_t lastUsed;
	float searchedPoints[RIGHT_LIMIT_DEG-LEFT_LIMIT_DEG][UPPER_LIMIT_DEG-LOWER_LIMIT_DEG]; //[X][Y]
};

class Tracking {
private:

private:
	pthread_mutex_t ballPhysicalParametersLock;
	pthread_mutex_t goalPhysicalParametersLock;
public:
	Head head;
private:
	BFSdata searchData;
	BallObject* ball;

private:
	bool checkElapsedTime(std::clock_t);

public:
	void setPointers(BallObject*);

	void centerBall(void);
	void searchBall(void);
	void BFSsearchBall(void);

	void centerBallExperimental(ballScreenParameters);

	void updatePhysicalParameters(ballScreenParameters);

	ballPhysicalParameters getBallPhysicalParameters(void);
	void setBallPhysicalParameters(ballPhysicalParameters);

	Tracking();
	virtual ~Tracking();
};

#endif /* TRACKING_H_ */
