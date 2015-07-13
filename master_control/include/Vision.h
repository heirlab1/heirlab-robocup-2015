/*
 * Vision.h
 *
 *  Created on: Jun 28, 2015
 *      Author: ryan
 */

#ifndef VISION_H_
#define VISION_H_

#include <pthread.h>

#include "Tracking.h"
#include "BallObject.h"
#include "FieldObject.h"
#include "Camera.h"

#define TASK_LOOK_FOR_BALL		0
#define TASK_LOOK_FOR_GOAL		1

class Vision {
	public:
		FieldObject fieldObject;
		BallObject ballObject;
		Tracking tracking;
		Camera camera;
	private:
		bool shutdown;
		int task;

		//Locks
		pthread_mutex_t shutdownLock;
		pthread_mutex_t taskLock;

	private:
		void* sightLoop(void*);
		void* motionLoop(void*);

	public:
		bool getShutdown(void);
		void setShutdown(bool);

		int getTask(void);
		void setTask(int);

		float getBallDistance(void);
		float getBallAngle(void);
		float getBallLastSeen(void);

		Vision(void);
		virtual ~Vision(void);
};



#endif /* VISION_H_ */
