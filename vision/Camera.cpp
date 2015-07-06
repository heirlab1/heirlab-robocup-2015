/*
 * Camera.cpp
 *
 *  Created on: Jul 2, 2015
 *      Author: ryan
 */

#include "Camera.h"

void Camera::captureFrame() {
	if(checkElapsedTime()) {
		capture.read(imageCameraFeed); //Capture new image
		cv::cvtColor(imageCameraFeed, imageHSV, cv::COLOR_BGR2HSV); //convert BGR(RGB) to HSV
		resetElapsedTime();
	}
}

cv::Mat Camera::getCameraImage() {
	captureFrame();
	return imageCameraFeed;
}

cv::Mat Camera::getHSVImage() {
	captureFrame();
	return imageHSV;
}

//Checks to see if time since last request has passed
bool Camera::checkElapsedTime() {
	std::clock_t currentTime = clock();
	if (float(currentTime-lastRequest)/CLOCKS_PER_SEC > minTime)
		return true;
	else
		return false;
}

//Resets time since last request to current time
void Camera::resetElapsedTime() {
	lastRequest = clock();
}

//Setup Frame
void Camera::setupCamera() {
	capture.open(0);
	//set height and width of capture frame
	capture.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);
	capture.read(imageCameraFeed); //Capture new image
}

Camera::Camera() {
	setupCamera();
}

Camera::~Camera() {
	capture.release();
}

