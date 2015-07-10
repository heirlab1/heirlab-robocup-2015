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


struct BFSdata {
	std::clock_t lastUsed;
	float searchedPoints[RIGHT_LIMIT_DEG-LEFT_LIMIT_DEG][UPPER_LIMIT_DEG-LOWER_LIMIT_DEG]; //[X][Y]
};

class Tracking {
private:
	float searchThreshold = 1; //(1=100% seen, 0.5=50% seen)
	int searchRadius = 20; //Area that will be marked as seen (Degrees)
private:
	pthread_mutex_t ballPhysicalParametersLock;
	pthread_mutex_t goalPhysicalParametersLock;
public:
	Head head;
private:
	BFSdata searchData;
	BallObject* ball;
	float maxTime = 0.2; //Min time between dynamixel communication bursts (1 = 1s, 0.5 = 500ms)

private:
	bool checkElapsedTime(std::clock_t);

public:
	void setPointers(BallObject*);

	void centerBall(void);
	void searchBall(void);
	void BFSsearchBall(void);

	void centerBallExperimental(void);

	ballPhysicalParameters getBallPhysicalParameters(void);
	void setBallPhysicalParameters(ballPhysicalParameters);



	Tracking();
	virtual ~Tracking();
};

#endif /* TRACKING_H_ */
