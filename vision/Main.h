/*
 * Main.h
 *
 *  Created on: Jul 6, 2015
 *      Author: ryan
 */

#ifndef MAIN_H_
#define MAIN_H_
#include "Vision.h"
#include <pthread.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

Vision vision;

void* sightLoop(void* arg) {
	//vision.tracking.head.setPanAngle(-47);
	//vision.tracking.head.setTiltAngle(0);

	while(1) {
		//vision.ballObject.detect(vision.camera.getCameraImage());
		//std::cout<<"Pan: "<<vision.tracking.head.motorManager.getMotorPosition(24)<<std::endl;
		//std::cout<<"fdsf"<<std::endl;
		//Vision vision2;
		vision.ballObject.detect(vision.camera.getCameraImage());
		//cv::Mat imageHSV = vision.camera.getHSVImage();
		//vision.ballObject.detect(imageHSV);
		//goalObject.detect(imageHSV);
	}
	pthread_exit(NULL);
}

void* motionLoop(void* arg) {
	//vision.tracking.head.setPanAngle(0);
	//vision.tracking.head.setTiltAngle(0);
	ballScreenParameters ball;

	while(1) {
		ballScreenParameters tempBall = vision.ballObject.getScreenParameters();
		if(tempBall.onScreen) {
			std::cout<<"OnScreen"<<std::endl;
			if(FRAME_WIDTH/3<tempBall.x && tempBall.x<FRAME_WIDTH*2/3<tempBall.y &&FRAME_HEIGHT/3<tempBall.x && tempBall.x<FRAME_HEIGHT*2/3<tempBall.x) {
				ballPhysicalParameters tempBallP;
				tempBallP.angle =vision.tracking.head.getPanAngle();
				tempBallP.distance = std::sqrt(std::pow(std::acos(std::abs(vision.tracking.head.getTiltAngle())-10)*.86, 2)-std::pow(.86, 2));
				tempBallP.timeStamp = clock();
				vision.ballObject.setPhysicalParameters(tempBallP);
				std::cout<<"Set physics"<<std::endl;
			}

			vision.tracking.centerBall();
		}
		else {
			std::cout<<"Searching"<<std::endl;
			vision.tracking.searchBall();
		}
	}
	pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
	//while(1) {
		//vision.ballObject.detect(vision.camera.getCameraImage());
	//}
		//cv::waitKey(30);
		//cv::imshow("feed", vision.camera.getHSVImage());
		//std::cout<<vision.getShutdown()<<std::endl;
		//vision.setShutdown(false);
			//goalObject.detect(imageHSV);
	//}

	//while(1) {
		//cv::waitKey(30);
		//cv::imshow("feed", vision.camera.getCameraImage());
	//}

	pthread_t sight, motion;
	pthread_create(&sight, NULL, sightLoop, NULL);
	//std::cout<<"Here"<<std::endl;
	pthread_create(&motion, NULL, motionLoop, NULL);
	pthread_join(sight, NULL);
};

class Main {
public:
	Main();
	virtual ~Main();
};

#endif /* MAIN_H_ */
