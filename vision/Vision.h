/*
 * Vision.h
 *
 *  Created on: Jun 28, 2015
 *      Author: ryan
 */

#ifndef VISION_H_
#define VISION_H_

#include "pthread.h"

#include "Tracking.h"
#include "BallObject.h"
#include "FieldObject.h"
#include "Camera.h"

#define TASK_LOOK_FOR_BALL
#define TASK_LOOK_FOR_GOAL

class Vision {
	private:
		FieldObject fieldObject;
		BallObject ballObject;
		Tracking tracking;
		Camera camera;

		bool shutdown;
		int task;

		//Locks
		pthread_mutex_t shutdownLock;
		pthread_mutex_t taskLock;

	void startThread(void);


	private:
		void* sightLoop(void*);
		void* motionLoop(void*);

		bool getShutdown(void);
		void setShutdown(bool);

	public:
		int getTask(void);
		void setTask(int);

		float getBallDistance(void);
		float getBallAngle(void);
		float getBallLastSeen(void);

		void killThread(void);

		Vision(void);
		virtual ~Vision(void);
};



#endif /* VISION_H_ */
