/*
 * Vision.h
 *
 *  Created on: Jun 28, 2015
 *      Author: ryan
 */

#ifndef VISION_H_
#define VISION_H_

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <ctime>

struct ballParameters {
	bool found = false;
	int x, y, radius;
};

struct goalParameters {
	bool found = false;
	int x, y;
};

struct FieldParameters {
	bool found = false;
	cv::Rect fieldBoundries;
};

class Vision {
	private:
		float minTime = 0.2; //Min time between capturing new image from capturing new image and processing (1 = 1s, 0.5 = 500ms)
		std::clock_t lastRequest;

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

		const int ERODE_KERNAL_SIZE = 2;
		const int DIALATE_KERNAL_SIZE = 2;
		const int BLUR_KERNAL_SIZE = 15; //Amount of blurring of original image (Notice: must be odd)

		//Detector Tools
		cv::Mat erodeElement; //Erodes whitespaces to get rid of background noise
		cv::Mat dilateElement ;//Dialates whitespaces to make bigger and fill voids

		//Video Frame
		const cv::string windowCameraFeed = "Original Image";
		const cv::string windowHSV = "HSV Image";
		const cv::string windowThreshold = "Thresholded Image";

		//Ball primary color calibration variables
		int BALL_H_MIN = 141;
		int BALL_H_MAX = 178;
		int BALL_S_MIN = 48;
		int BALL_S_MAX = 256;
		int BALL_V_MIN = 115;
		int BALL_V_MAX = 256;

		//Ball secondary color calibration variables
		int BALL2_H_MIN = 141;
		int BALL2_H_MAX = 178;
		int BALL2_S_MIN = 48;
		int BALL2_S_MAX = 256;
		int BALL2_V_MIN = 115;
		int BALL2_V_MAX = 256;

		//Grass calibration variables
		int GRASS_H_MIN = 34;
		int GRASS_H_MAX = 86;
		int GRASS_S_MIN = 125;
		int GRASS_S_MAX = 256;
		int GRASS_V_MIN = 37;
		int GRASS_V_MAX = 256;

		//Display Overlay Colors
		cv::Scalar TextColor = cv::Scalar(255,255,0);
		cv::Scalar HighlightColor = cv::Scalar(0,255,0);
		cv::Scalar GridColor = cv::Scalar(0,0,255);
		cv::Scalar CircleColor = cv::Scalar(255,0,0);
		cv::Scalar CircleCenterColor = cv::Scalar(0,255,0);
		cv::Vec4f previous_found = cv::Vec4f(0,0,0,0);



		void setupFrame(void);


		void captureFrame(void);
		void preprocessFrame(void);

		bool checkElapsedTime(void);
		void resetElapsedTime(void);

		cv::Point findBallThreshold(void);
		cv::Point findBallArea(void);
		cv::Rect findField(void);



		cv::Mat fillHoles(cv::Mat);

		void displayFeed(void);


		void detectGoal(void);

	public:
		void processFrame(void);
		ballParameters getBallLocation(int);
		goalParameters getGoalLocation(int);

		Vision();
		virtual ~Vision(void);
};

#endif /* VISION_H_ */
