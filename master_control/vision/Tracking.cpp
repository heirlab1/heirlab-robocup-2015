/*
 * Tracking.cpp
 *
 *  Created on: Jun 29, 2015
 *      Author: ryan
 */

#include "Tracking.h"

void Tracking::centerBall() {
	ballScreenParameters tempBall = vision.getBallScreenParameters();

	if(vision.FRAME_WIDTH/3 < tempBall.x && tempBall.x < vision.FRAME_WIDTH*2/3 && vision.FRAME_HEIGHT/3 < tempBall.y && tempBall.y < vision.FRAME_HEIGHT*2/3)
		std::cout<<"Center"<<std::endl;
	else {
		if(tempBall.x<vision.FRAME_WIDTH/3) {
			std::cout<<"Right"<<std::endl;
			head.moveHead(DIRECTION_RIGHT);
			while(tempBall.x<vision.FRAME_WIDTH/3) {
				tempBall = vision.getBallScreenParameters();
			}
			head.stopHead();
		}
		else if (tempBall.x>vision.FRAME_WIDTH*2/3) {
			std::cout<<"Left"<<std::endl;
			head.moveHead(DIRECTION_LEFT);
			while(tempBall.x>vision.FRAME_WIDTH*2/3) {
				tempBall = vision.getBallScreenParameters();
			}
			head.stopHead();
		}
		else if(tempBall.y<vision.FRAME_HEIGHT/3) {
			std::cout<<"Up"<<std::endl;
			head.moveHead(DIRECTION_UP);
			while(tempBall.y<vision.FRAME_HEIGHT/3) {
				tempBall = vision.getBallScreenParameters();
			}
			head.stopHead();
		}
		else if (tempBall.y>vision.FRAME_HEIGHT*2/3) {
			std::cout<<"Down"<<std::endl;
			head.moveHead(DIRECTION_DOWN);
			while(tempBall.y>vision.FRAME_HEIGHT*2/3) {
				tempBall = vision.getBallScreenParameters();
			}
			head.stopHead();
		}
	}
}

Tracking::Tracking() {
	// TODO Auto-generated constructor stub

}

Tracking::~Tracking() {
	// TODO Auto-generated destructor stub
}

