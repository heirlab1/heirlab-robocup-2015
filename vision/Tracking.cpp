/*
 * Tracking.cpp
 *
 *  Created on: Jun 29, 2015
 *      Author: ryan
 */

#include "Tracking.h"

#define FRAME_WIDTH		640
#define FRAME_HEIGHT	480



void Tracking::searchBall() {

}

void Tracking::centerBall() {
	ballScreenParameters tempBall = ball->getScreenParameters();

	if(FRAME_WIDTH/3 < tempBall.x && tempBall.x < FRAME_WIDTH*2/3 && FRAME_HEIGHT/3 < tempBall.y && tempBall.y < FRAME_HEIGHT*2/3)
		std::cout<<"Center"<<std::endl;
	else {
		if(tempBall.x<FRAME_WIDTH/3) {
			std::cout<<"Right"<<std::endl;
			head.moveHead(DIRECTION_RIGHT);
			while(tempBall.x<FRAME_WIDTH/3) {
				tempBall = ball->getScreenParameters();
			}
			head.stopHead();
		}
		else if (tempBall.x>FRAME_WIDTH*2/3) {
			std::cout<<"Left"<<std::endl;
			head.moveHead(DIRECTION_LEFT);
			while(tempBall.x>FRAME_WIDTH*2/3) {
				tempBall = ball->getScreenParameters();
			}
			head.stopHead();
		}
		else if(tempBall.y<FRAME_HEIGHT/3) {
			std::cout<<"Up"<<std::endl;
			head.moveHead(DIRECTION_UP);
			while(tempBall.y<FRAME_HEIGHT/3) {
				tempBall = ball->getScreenParameters();
			}
			head.stopHead();
		}
		else if (tempBall.y>FRAME_HEIGHT*2/3) {
			std::cout<<"Down"<<std::endl;
			head.moveHead(DIRECTION_DOWN);
			while(tempBall.y>FRAME_HEIGHT*2/3) {
				tempBall = ball->getScreenParameters();
			}
			head.stopHead();
		}
	}
}


void Tracking::setPointers(BallObject* tempBall) {
	ball = tempBall;
}

Tracking::Tracking() {
}

Tracking::~Tracking() {
}

