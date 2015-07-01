#include "VisionModule.h"
#include <unistd.h>

#include <string>
#include <sstream>
#include <stdlib.h>
#include <vector>
#include <iomanip>
#include <cmath>
#include <iostream>
#include <fstream>
#include <map>

//#include "opencv.hpp"
//#include <stdio.h>
//#include <stdlib.h>

//#include "features2d.hpp"

void VisionModule::setupFrame() {
	capture.open(0);

	//set height and width of capture frame
	capture.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);
}

	// Detects a ball via the threshold method
cv::Point VisionModule::detectBallThreshold() {
	cv::waitKey(30);
	bool foundBall = false; //Ball assumed not found

	capture.read(imageCameraFeed); //Read from camera feed
	cv::cvtColor(imageCameraFeed, imageHSV, cv::COLOR_BGR2HSV); //convert BGR(RGB) to HSV
	cv::blur(imageHSV, imageHSV, cv::Size(10,10));
	cv::inRange(imageHSV, cv::Scalar(BALL_H_MIN, BALL_S_MIN, BALL_V_MIN), cv::Scalar(BALL_H_MAX, BALL_S_MAX, BALL_V_MAX), imageThreshold); // Filter image according to ball thresholds
	erodeElement = getStructuringElement(cv::MORPH_RECT, cv::Size(4,4)); //Dialate and Erode to rid background noise and make clearer
	dilateElement = getStructuringElement(cv::MORPH_RECT, cv::Size(8,8)); //^Also use rectangles because faster than circles

	//Erode and dialate set amount of times
	for(int i = 0; i<THRESH_ERODE_LIMIT; i++) {
		cv::erode(imageThreshold, imageThreshold, erodeElement);
	}
	for(int i = 0; i<THRESH_DIALATE_LIMIT; i++) {
		cv::dilate(imageThreshold, imageThreshold, dilateElement);
	}

	cv::vector<cv::Vec3f> detectedCircles;
	cv::HoughCircles(imageThreshold, detectedCircles, CV_HOUGH_GRADIENT, 2, imageThreshold.rows/8, 100, 50, 0, 150);

	int foundCircleIndex = 0;
	for(size_t i = 0; i<detectedCircles.size(); i++) {
		foundBall=true;
		cv::Point circleCenter(cvRound(detectedCircles[i][0]), cvRound(detectedCircles[i][1]));
		int circleRadius = cvRound(detectedCircles[i][2]);
		cv::circle(imageCameraFeed, circleCenter, circleRadius, cv::Scalar(0, 255, 0), 5);

		if(detectedCircles[foundCircleIndex][2]<detectedCircles[i][2]) { //Find circle with largest radius and put that in output
			foundCircleIndex = i;
		}
	}

	cv::Point ball;
	if(foundBall) {
		int circleRadius = cvRound(detectedCircles[foundCircleIndex][2]);
		ball = cv::Point(detectedCircles[foundCircleIndex][0], detectedCircles[foundCircleIndex][1]);
		//cv::putText(imageCameraFeed, cv::Point(detectedCircles[foundCircleIndex][0], detectedCircles[foundCircleIndex][0]+50),2,1, cv::Scalar(0,255,0),2);

		//std::string output;
		//output<<ball.x<<" "<<ball.y;


		//cv::putText(imageCameraFeed, output, cv::Point(0,50),1,2,cv::Scalar(0,0,255),2);
	}
	else {
		//cv::putText(imageCameraFeed, "No ball found", cv::Point(0,50),1,2,cv::Scalar(0,0,255),2);
		ball = cv::Point(-1,-1);
	}
	//std::cout<<"Ball location: "<<ball<<std::endl;
	displayFeed(); //Uncomment to view image processes for this section of code
	return ball;
}

/*
		cv::SimpleBlobDetector::Params params;

		// Filter by Circularity
		params.filterByCircularity = true;
		params.minCircularity = 0.8;

		// Set up detector with params
		cv::SimpleBlobDetector blobDetector(params);
		blobDetector.detect()
*/

void VisionModule::displayFeed() {
	//show frames
	imshow(windowThreshold, imageThreshold); //Show threshold image
	imshow(windowCameraFeed, imageCameraFeed); //Show camera feed image
	//imshow(windowHSV, imageHSV); //Show HSV image
}

	/*cv::Rect findField(cv::Mat frame);
		std::vector<cv::Vec4f> sobelDetect(cv::Mat frame, std::vector<cv::Vec4f> circle_points);
		std::vector<cv::Vec4f> grayDetect(cv::Mat frame, std::vector<cv::Vec4f> circle_points);
		std::vector<cv::Vec4f> threshDetect(cv::Mat frame, std::vector<cv::Vec4f> circle_points);
		std::vector<cv::Vec4f> overlayDetect(cv::Mat frame, std::vector<cv::Vec4f> circle_points, cv::SimpleBlobDetector detector);
		std::vector<cv::Vec4f> contoursDetect(cv::Mat frame, std::vector<cv::Vec4f> circle_points);
		void placeGrid(cv::Mat frame);
		void checkLocation(cv::Mat frame, cv::Point center_rec);
	 */


void VisionModule::detectGoal() {

}

//Constructor
VisionModule::VisionModule() {
	setupFrame();
}

//Destructor
VisionModule::~VisionModule() {
	capture.release(); // Release the video
}

//Main Method
int main(int argc, char* argv[]) {
	HeadModule head;

	VisionModule vision;

	//head.scan();

	/*if(head.motorsCheckMoving()){
		printf("moving");
	}
	else printf("not moving");*/

	int offset = 0;
	int id = 0;

	//head.motorMoveTo(TILT_MOTOR_ID, -100);
	//std::cout<<head.motorsReadPosition()<<std::endl;
	//head.motorMoveTo(PAN_MOTOR_ID, 1000);

	std::cout<<"Now"<<std::endl;
	/*usleep(1000*5000);
	head.motorIncrement(24, -50);
	head.motorIncrement(24, 100);
	usleep(1000*5000);
	head.motorIncrement(24, -50);
	std::cout<<"End"<<std::endl;*/

	/*while(1) {
		//usleep(1000*4000);
		cv::Point currPos = head.motorsReadPosition();
		std::cout<<currPos<<std::endl;
		if (currPos.x == rightLimit && currPos.y == upperLimit)
			head.motorsMoveTo(cv::Point(0,0));
		else if(currPos.x == 0 && currPos.y == 0)
			head.motorsMoveTo(cv::Point(rightLimit-leftLimit,upperLimit-lowerLimit));
		else if(currPos.x == rightLimit-leftLimit && currPos.y == upperLimit-lowerLimit)
			head.motorsMoveTo(cv::Point(0,0));

		head.motorsWaitForStop();
	}*/

	//while(1) {
	//	vision.detectBallThreshold();
	//}


	while(1) {
		cv::Point ballPos = vision.detectBallThreshold();
		if (ballPos.y != -1 && ballPos.x != -1) {
			if(vision.FRAME_WIDTH/3 < ballPos.x && ballPos.x < vision.FRAME_WIDTH*2/3 && vision.FRAME_HEIGHT/3 < ballPos.y && ballPos.y < vision.FRAME_HEIGHT*2/3)
				std::cout<<"Center"<<std::endl;
			else {
				if(ballPos.x<vision.FRAME_WIDTH/3) {
					std::cout<<"Right"<<std::endl;
					head.motorMoveTo(PAN_MOTOR_ID, rightLimit-leftLimit);
					while(ballPos.x<vision.FRAME_WIDTH/3) {
						ballPos = vision.detectBallThreshold();
					}
					head.stopHead();
				}
				else if (ballPos.x>vision.FRAME_WIDTH*2/3) {
					std::cout<<"Left"<<std::endl;
					head.motorMoveTo(PAN_MOTOR_ID, 0);
					while(ballPos.x>vision.FRAME_WIDTH*2/3) {
						ballPos = vision.detectBallThreshold();
					}
					head.stopHead();
				}
				else if(ballPos.y<vision.FRAME_HEIGHT/3) {
					std::cout<<"Up"<<std::endl;
					head.motorMoveTo(TILT_MOTOR_ID, 0);
					while(ballPos.y<vision.FRAME_HEIGHT/3) {
						ballPos = vision.detectBallThreshold();
					}
					head.stopHead();
				}
				else if (ballPos.y>vision.FRAME_HEIGHT*2/3) {
					std::cout<<"Down"<<std::endl;
					head.motorMoveTo(TILT_MOTOR_ID, upperLimit-lowerLimit-1);
					while(ballPos.y>vision.FRAME_HEIGHT*2/3) {
						ballPos = vision.detectBallThreshold();
					}
					head.stopHead();
				}
				usleep(1000*1000);
			}
		}
	}

	// Current Vision method
	/*while(0) {
		//usleep(1000*20);
		//cv::waitKey(30);

		cv::Point ballPos = vision.detectBallThreshold();

		//head.motorsMoveTo(cv::Point(rightLimit, upperLimit));

		if (ballPos.y != -1 && ballPos.x != -1) {
			if(vision.FRAME_WIDTH/3 < ballPos.x && ballPos.x < vision.FRAME_WIDTH*2/3 && vision.FRAME_HEIGHT/3 < ballPos.y && ballPos.y < vision.FRAME_HEIGHT*2/3)
				std::cout<<"Center"<<std::endl;
			else {
				if(ballPos.x<vision.FRAME_WIDTH/3) {
					offset = 40;
					std::cout<<"Right"<<std::endl;
					id = PAN_MOTOR_ID;
				}
				else if (ballPos.x>vision.FRAME_WIDTH*2/3) {
					offset = -40;
					std::cout<<"Left"<<std::endl;
					id = PAN_MOTOR_ID;
				}
				else if(ballPos.y<vision.FRAME_HEIGHT/3) {
					offset = -40;
					std::cout<<"Up"<<std::endl;
					id = TILT_MOTOR_ID;
				}
				else if (ballPos.y>vision.FRAME_HEIGHT*2/3) {
					offset = 40;
					std::cout<<"Down"<<std::endl;
					id = TILT_MOTOR_ID;
				}
				std::cout<<" "<< head.motorsReadPosition();
				head.motorIncrement(id, offset);
				head.motorsWaitForStop();
				//usleep(1000*100);
			}
		}
	}*/
}
