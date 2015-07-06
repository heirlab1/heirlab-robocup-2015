/*
 * Main.h
 *
 *  Created on: Jul 6, 2015
 *      Author: ryan
 */

#ifndef MAIN_H_
#define MAIN_H_
#include "Vision.h"
#include "pthread.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>

Vision vision;

void* sightLoop(void* arg) {
	while(!vision.getShutdown()) {
		cv::Mat imageHSV = vision.camera.getHSVImage();
		vision.ballObject.detect(imageHSV);
		//goalObject.detect(imageHSV);
	}
	pthread_exit(NULL);
}

void* motionLoop(void* arg) {
	while(!vision.getShutdown()) {
		if(!vision.ballObject.getScreenParameters().onScreen)
			vision.tracking.searchBall();
		else
			vision.tracking.centerBall();
	}
	pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
	pthread_t sight;
	pthread_create(&sight, NULL, sightLoop, NULL);
};

class Main {
public:
	Main();
	virtual ~Main();
};

#endif /* MAIN_H_ */
