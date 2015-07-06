/*
 * Vision.cpp
 *
 *  Created on: Jun 28, 2015
 *      Author: ryan
 */

#include "Vision.h"
#include "pthread.h"

// Returns distance of the ball from the between the feet in METERS
float Vision::getBallDistance() {
	return ballObject.getPhysicalParameters().distance;
}

// Returns the angle of the ball if on screen in DEGREES
// 0 is straight ahead, negative is to the left, positive is to the right
float Vision::getBallAngle() {
	return ballObject.getPhysicalParameters().angle;
}

// Returns last time the ball was seen in SECONDS
float Vision::getBallLastSeen() {
	return ballObject.getPhysicalParameters().timeStamp;
}


void Vision::startThread( ) {
	std::cout<<"Started Threads"<<std::endl;
	setShutdown(false);
	//pthread_create(&motion, NULL, motionLoop, NULL);
}

void Vision::killThread() {
	std::cout<<"killing threads"<<std::endl;
	setShutdown(true);
}

void* Vision::sightLoop(void* arg) {
	while(!getShutdown()) {
		cv::Mat imageHSV = camera.getHSVImage();
		ballObject.detect(imageHSV);
		//goalObject.detect(imageHSV);
	}
	pthread_exit(NULL);
}

void* Vision::motionLoop(void* arg) {
	while(!getShutdown()) {
		if(!ballObject.getScreenParameters().onScreen)
			tracking.searchBall();
		else
			tracking.centerBall();
	}
	pthread_exit(NULL);
}

bool Vision::getShutdown() {
	pthread_mutex_lock(&shutdownLock);
	bool tempShutdown = shutdown;
	pthread_mutex_unlock(&shutdownLock);
	return tempShutdown;
}

void Vision::setShutdown(bool tempShutdown) {
	pthread_mutex_lock(&shutdownLock);
	shutdown = tempShutdown;
	pthread_mutex_unlock(&shutdownLock);
}

int Vision::getTask() {
	pthread_mutex_lock(&taskLock);
	int tempTask = task;
	pthread_mutex_unlock(&taskLock);
	return tempTask;
}

void Vision::setTask(int tempTask) {
	pthread_mutex_lock(&taskLock);
	task = tempTask;
	pthread_mutex_unlock(&taskLock);
}

Vision::Vision() {
	ballObject.setPointers(&fieldObject);
	tracking.setPointers(&ballObject);
	startThread();
}

Vision::~Vision() {
	setShutdown(true);
}

int main(int argc, char* argv[])
{
	pthread_t sight, motion;
	Vision vision;
	pthread_create(&sight, NULL, vision.sightLoop, NULL);

	pthread_join(sight, NULL);
	pthread_join(motion, NULL);
}
