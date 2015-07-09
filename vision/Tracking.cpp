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

void Tracking::centerBallExperimental() {
	ballScreenParameters tempBall = ball->getScreenParameters();
		if(tempBall.onScreen && !head.motorManager.checkMoving(23) && !head.motorManager.checkMoving(24)) {
			float pan = head.getPanAngle();
			float tilt = head.getTiltAngle();

			//pan+= -(tempBall.x-(FRAME_WIDTH/2))*(70/2);
			//tilt+= (tempBall.y-(FRAME_WIDTH/2))*(50/2);
			float x = (tempBall.x-(FRAME_WIDTH/2))*16/162;
			float y =(tempBall.y-(FRAME_HEIGHT/2))*16/162;
			std::cout<<x<<", "<<y<<std::endl;
			pan+=x;
			tilt+=y;

			head.setPanAngle(x);
			head.setTiltAngle(y);
		}
}

void Tracking::centerBall() {
	ballScreenParameters tempBall = ball->getScreenParameters();
	if(tempBall.onScreen) {
		if(FRAME_WIDTH/3 < tempBall.x && tempBall.x < FRAME_WIDTH*2/3 && FRAME_HEIGHT/3 < tempBall.y && tempBall.y < FRAME_HEIGHT*2/3) {
			//std::cout<<"Center"<<std::endl;
			//head.stopHead();
		}
		else {
			clock_t timeStart = clock();
			if(tempBall.x<FRAME_WIDTH/3) {
				std::cout<<"Left"<<std::endl;
				head.moveHead(DIRECTION_LEFT);
				while(tempBall.x<FRAME_WIDTH/3 && !checkElapsedTime(timeStart) && tempBall.onScreen) {
					tempBall = ball->getScreenParameters();
				}
				head.stopHead();
			}
			else if (tempBall.x>FRAME_WIDTH*2/3) {
				std::cout<<"Right"<<std::endl;
				head.moveHead(DIRECTION_RIGHT);
				while(tempBall.x>FRAME_WIDTH*2/3 && !checkElapsedTime(timeStart) && tempBall.onScreen) {
					tempBall = ball->getScreenParameters();
				}
				head.stopHead();
			}
			else if(tempBall.y<FRAME_HEIGHT/3) {
				std::cout<<"Up"<<std::endl;
				head.moveHead(DIRECTION_UP);
				while(tempBall.y<FRAME_HEIGHT/3 && !checkElapsedTime(timeStart) && tempBall.onScreen) {
					tempBall = ball->getScreenParameters();
				}
				head.stopHead();
			}
			else if (tempBall.y>FRAME_HEIGHT*2/3) {
				std::cout<<"Down"<<std::endl;
				head.moveHead(DIRECTION_DOWN);
				while(tempBall.y>FRAME_HEIGHT*2/3 && !checkElapsedTime(timeStart) && tempBall.onScreen) {
					tempBall = ball->getScreenParameters();
				}
				head.stopHead();
			}
		}
	}
}

//Checks to see if time since last request has passed
bool Tracking::checkElapsedTime(std::clock_t timeRequest) {
	std::clock_t currentTime = clock();
	if ((currentTime-timeRequest)/CLOCKS_PER_SEC > maxTime)
		return true;
	else
		return false;
}


void Tracking::setPointers(BallObject* tempBall) {
	ball = tempBall;
}

Tracking::Tracking() {
}

Tracking::~Tracking() {
}

