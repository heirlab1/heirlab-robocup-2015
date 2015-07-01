/*
 * Vision.h
 *
 *  Created on: Jun 10, 2015
 *      Author: ryan
 */

#ifndef VISIONMODULE_H_
#define VISIONMODULE_H_

#include <sstream>
#include <string>
#include <iostream>

#include <opencv/highgui.h>
#include <opencv/cv.h>

#include "HeadModule.h"

class VisionModule
{
	private:

	public:
	//Video Properties
	const int FRAME_WIDTH = 640;
	const int FRAME_HEIGHT = 480;
	cv::Mat imageCameraFeed; //Direct Camera Image
	cv::Mat imageHSV; //HSV Image
	cv::Mat imageThreshold; //Threshold Image
	cv::VideoCapture capture; //Video capture object required for acquiring videocam feed

	//Detector Variables
	const int THRESH_ERODE_LIMIT = 3;
	const int THRESH_DIALATE_LIMIT = 2;

	//Detector Tools
	cv::Mat erodeElement; //Erodes whitespaces to get rid of background noise
	cv::Mat dilateElement ;//Dialates whitespaces to make bigger and fill voids

	//Video Frame
	const cv::string windowCameraFeed = "Original Image";
	const cv::string windowHSV = "HSV Image";
	const cv::string windowThreshold = "Thresholded Image";

	//Ball Calibration Variables
	int BALL_H_MIN = 141;
	int BALL_H_MAX = 178;
	int BALL_S_MIN = 48;
	int BALL_S_MAX = 256;
	int BALL_V_MIN = 115;
	int BALL_V_MAX = 256;

	//Display Overlay Colors
	cv::Scalar TextColor = cv::Scalar(255,255,0);
	cv::Scalar HighlightColor = cv::Scalar(0,255,0);
	cv::Scalar GridColor = cv::Scalar(0,0,255);
	cv::Scalar CircleColor = cv::Scalar(255,0,0);
	cv::Scalar CircleCenterColor = cv::Scalar(0,255,0);
	cv::Vec4f previous_found = cv::Vec4f(0,0,0,0);

	VisionModule();
	virtual ~VisionModule(void);

	void setupFrame(void);
	cv::Point detectBallThreshold(void);
	void displayFeed(void);
	void detectGoal(void);
};



#endif /* VISIONMODULE_H_ */
