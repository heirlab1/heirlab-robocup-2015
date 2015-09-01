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
#include <unistd.h>

Vision vision;


/*void* sightLoop(void* arg) {
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
}*/

/*void* motionLoop(void* arg) {
	//vision.tracking.head.setPanAngle(-7);
	//vision.tracking.head.setTiltAngle(-19);
	//ballScreenParameters ball;

	while(1) {
		//vision.tracking.centerBallExperimental();
		//usleep(1000*100);
		//vision.tracking.searchBall();
		ballScreenParameters tempBall = vision.ballObject.getScreenParameters();
		vision.tracking.updatePhysicalParameters(tempBall);
		vision.tracking.centerBallExperimental(tempBall);
		//vision.tracking.head.setTiltAngle(0);
		//std::cout<<vision.tracking.head.getTiltAngle()<<std::endl;
		std::cout<<vision.ballObject.getPhysicalParameters().distance<<std::endl;
		if(tempBall.onScreen) {
			std::cout<<"Centering"<<std::endl;
			vision.tracking.head.motorManager.setSpeed(23, 70);
			vision.tracking.centerBallExperimental(tempBall);
		}
		else {
			vision.tracking.head.motorManager.setSpeed(23, 20);
			vision.tracking.head.motorManager.setSpeed(24, 20);
			std::cout<<"Searching"<<std::endl;
			vision.tracking.searchBall();
		}
	}
	pthread_exit(NULL);
}*/

/*int main(int argc, char* argv[]) {
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
};*/


class Main {
public:
	Main();
	virtual ~Main();
};

#endif /* MAIN_H_ */
