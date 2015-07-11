/*
 * Tracking.cpp
 *
 *  Created on: Jun 29, 2015
 *      Author: ryan
 */

#include "Tracking.h"
#include <cmath>
#include <unistd.h>

#define FRAME_WIDTH		640
#define FRAME_HEIGHT	480


void Tracking::searchBall() {
	//int currentPoint = cv::Point(head.getPanAngle(), head.getTiltAngle());

	for(int x=-48; x<=48; x+=12) {
		int y = -0.01172*std::pow(x, 2)-15;
		head.setTiltAngle(y);
		head.setPanAngle(x);
		usleep(1000*2);
		while(head.motorManager.checkMoving(23) || head.motorManager.checkMoving(24)) {
			if(ball->getScreenParameters().onScreen) {
				head.stopHead();
				usleep(1000*200);
				if(ball->getScreenParameters().onScreen)
					break;
				else {
					head.setTiltAngle(y);
					head.setPanAngle(x);
				}
			}
		}
	}
}

//Scans for nearest point that unsearched enough and returns it (EXPERIMENTAL)
void Tracking::BFSsearchBall()  {

	std::queue<cv::Point> searchQueue;
	std::queue<cv::Point> finalQueue;

	bool BFSmatrix[RIGHT_LIMIT_DEG-LEFT_LIMIT_DEG][UPPER_LIMIT_DEG-LOWER_LIMIT_DEG]; //[X][Y]

	//Reset Searched area
	for(int i=0; i<RIGHT_LIMIT_DEG-LEFT_LIMIT_DEG; i++) { //Loop motorMatrix rows
		for(int j=0; j<UPPER_LIMIT_DEG-LOWER_LIMIT_DEG; j++) { //Loop motorMatrix columns
			BFSmatrix[i][j]=false; //[X][Y]
			searchData.searchedPoints[i][j]= 0;
		}
	}

	bool entireAreaSearched= false;

	//While ball not found and the entire area hasn't been searched
	while(!ball->getScreenParameters().onScreen && entireAreaSearched==false) {

		cv::Point lowestValuePoint, tempPoint; //lowestValuePoint is placeholder for return value
		tempPoint = cv::Point((int)head.getPanAngle()-LEFT_LIMIT_DEG, (int)head.getTiltAngle()-LOWER_LIMIT_DEG);
		while(RIGHT_LIMIT_DEG-LEFT_LIMIT_DEG<tempPoint.x || UPPER_LIMIT_DEG-LOWER_LIMIT_DEG<tempPoint.y) {
			std::cout<<"Failed"<<std::endl;
			tempPoint = cv::Point((int)head.getPanAngle()-LEFT_LIMIT_DEG, (int)head.getTiltAngle()-LOWER_LIMIT_DEG);
		}

		// Mark area around current point as seen -----------------------------------------------------------------------------------------------------

		for(int i=0; i<RIGHT_LIMIT_DEG-LEFT_LIMIT_DEG; i++) { //Loop motorMatrix rows
			for(int j=0; j<UPPER_LIMIT_DEG-LOWER_LIMIT_DEG; j++) { //Loop motorMatrix columns
				float distance = std::sqrt((tempPoint.x-i)*(tempPoint.x-i)+(tempPoint.y-j)*(tempPoint.y-j));
				//std::cout<<(int)distance<<"\t";
				if(distance<.10*searchRadius) //If in 10% of the center
					searchData.searchedPoints[i][j]=searchData.searchedPoints[i][j]+searchThreshold;
				else if(distance<.25*searchRadius) //If 25% from center
					searchData.searchedPoints[i][j]=searchData.searchedPoints[i][j]+searchThreshold;
				else if(distance<.50*searchRadius) //If 50% from center
					searchData.searchedPoints[i][j]=searchData.searchedPoints[i][j]+0.4*searchThreshold;
				else if(distance<.75*searchRadius) //If 75% from center
					searchData.searchedPoints[i][j]=searchData.searchedPoints[i][j]+0.1*searchThreshold;
			}
			//std::cout<<std::endl;
		}
		//std::cout<<std::endl;

		//Search area around point for unseen point -----------------------------------------------------------------------------------------------------
		searchQueue.push(tempPoint); //Push current point
		BFSmatrix[tempPoint.x][tempPoint.y]=true;

		//While nothing is found and the entire image hasn't been searched
		while(finalQueue.empty() && 0<searchQueue.size()) {
			//For this task of queue entries, process
			int searchQueueSize = searchQueue.size();

			//For all points within queue, search get and add to queue; If an unseen area(s) are found, the while loop will break
			for(int i=0; i<searchQueueSize; i++) {
				tempPoint = searchQueue.front(); //Take element from queue
				searchQueue.pop(); //Remove element from queue

				// If within a given threshold, add to final queue
				if(searchData.searchedPoints[tempPoint.x][tempPoint.y]<searchThreshold) {
					finalQueue.push(tempPoint);
					break;
				}
				else {
					//Searches adjacent points (up, down, right, left) in graph to see if they have been explored. If not, add to search queue
					//Outer if statements make sure we are not searching outside our head limitation angles
					//Inner if statements check to see if the elements we want to add have not previously been searched
					if(tempPoint.x< RIGHT_LIMIT_DEG-LEFT_LIMIT_DEG) { //Check right
						if(!BFSmatrix[tempPoint.x+1][tempPoint.y]) {
							searchQueue.push(cv::Point(tempPoint.x+1, tempPoint.y));
							BFSmatrix[tempPoint.x+1][tempPoint.y]=true;
						}
					}
					if(tempPoint.x-1 >= 0) {
						if(!BFSmatrix[tempPoint.x-1][tempPoint.y]) { //Check left
							searchQueue.push(cv::Point(tempPoint.x-1, tempPoint.y));
							BFSmatrix[tempPoint.x-1][tempPoint.y]=true;
						}
					}
					if(tempPoint.y+1 < UPPER_LIMIT_DEG-LOWER_LIMIT_DEG) { //Check up
						if(!BFSmatrix[tempPoint.x][tempPoint.y+1]) {
							searchQueue.push(cv::Point(tempPoint.x, tempPoint.y+1));
							BFSmatrix[tempPoint.x][tempPoint.y+1]=true;
						}
					}
					if(tempPoint.y-1 >= 0) { //Check down
						if(!BFSmatrix[tempPoint.x][tempPoint.y-1]) {
							searchQueue.push(cv::Point(tempPoint.x, tempPoint.y-1));
							BFSmatrix[tempPoint.x][tempPoint.y-1]=true;
						}
					}
				}//End else
				for(int i=0; i<(RIGHT_LIMIT_DEG-LEFT_LIMIT_DEG); i++) { //Loop motorMatrix rows
					for(int j=0; j<(UPPER_LIMIT_DEG-LOWER_LIMIT_DEG); j++) { //Loop motorMatrix columns
						std::cout<<searchData.searchedPoints[i][j]<<"\t";
					}
					std::cout<<std::endl;
				}
				std::cout<<std::endl;

			}//End For
		} //End while

		//Select most unseen area from the finalQueue -----------------------------------------------------------------------------------------

		if(!finalQueue.empty()) {
			lowestValuePoint = finalQueue.front(); //place
			finalQueue.pop();
		}

		//std::cout<<"GOTO: "<<lowestValuePoint.x+LEFT_LIMIT_DEG<<", "<<lowestValuePoint.y+LOWER_LIMIT_DEG<<std::endl;

		//Set pan/tilt to that point and wait for robot to compete move -----------------------------------------------------------------------------

		head.setPanAngle(lowestValuePoint.x+LEFT_LIMIT_DEG);
		head.setTiltAngle(lowestValuePoint.y+LOWER_LIMIT_DEG);

		while(head.motorManager.checkMoving(23) || head.motorManager.checkMoving(24)); // wait for head to stop
	}
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
			if(tempBall.x<FRAME_WIDTH*2/5) {
				std::cout<<"Left"<<std::endl;
				head.moveHead(DIRECTION_LEFT);
				while(tempBall.x<FRAME_WIDTH/3 && !checkElapsedTime(timeStart) && tempBall.onScreen) {
					tempBall = ball->getScreenParameters();
				}
				head.stopHead();
			}
			else if (tempBall.x>FRAME_WIDTH*2/5) {
				std::cout<<"Right"<<std::endl;
				head.moveHead(DIRECTION_RIGHT);
				while(tempBall.x>FRAME_WIDTH*2/3 && !checkElapsedTime(timeStart) && tempBall.onScreen) {
					tempBall = ball->getScreenParameters();
				}
				head.stopHead();
			}
			else if(tempBall.y<FRAME_HEIGHT*2/5) {
				std::cout<<"Up"<<std::endl;
				head.moveHead(DIRECTION_UP);
				while(tempBall.y<FRAME_HEIGHT/3 && !checkElapsedTime(timeStart) && tempBall.onScreen) {
					tempBall = ball->getScreenParameters();
				}
				head.stopHead();
			}
			else if (tempBall.y>FRAME_HEIGHT*2/5) {
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

