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
#include <mutex>


/*
//Video properties
#define FRAME_WIDTH 	640
#define FRAME_HEIGHT	480

//Ball primary color calibration variables
#define BALL0_H_MIN		0
#define BALL0_H_MAX		134
#define BALL0_S_MIN		0
#define BALL0_S_MAX		69
#define BALL0_V_MIN		124
#define BALL0_V_MAX		249

//Ball secondary color calibration variables
#define BALL1_H_MIN		0
#define BALL1_H_MAX		189
#define BALL1_S_MIN		0
#define BALL1_S_MAX		141
#define BALL1_V_MIN		0
#define BALL1_V_MAX		95

//Grass calibration variables
#define GRASS_H_MIN		34
#define GRASS_H_MAX		86
#define GRASS_S_MIN		125
#define GRASS_S_MAX		256
#define GRASS_V_MIN		37
#define GRASS_V_MAX		256
*/

struct ballScreenParameters {
	bool found;
	int x, y, radius;
};

struct goalScreenParameters {
	bool found;
	int x, y;
};

struct fieldScreenParameters {
	bool found;
	cv::Rect boundries;
};

class Vision {
	public:
		const int FRAME_WIDTH = 640;
		const int FRAME_HEIGHT = 480;

	private:
		pthread_mutex_t ballScreenParametersLock;
		pthread_mutex_t goalScreenParametersLock;

		ballScreenParameters ball;
		fieldScreenParameters field;
		goalScreenParameters goal;

		float minTime = 0.2; //Min time between capturing new image from capturing new image and processing (1 = 1s, 0.5 = 500ms)
		std::clock_t lastRequest;

		//Video Properties
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
		int BALL0_H_MIN = 0;
		int BALL0_H_MAX = 134;
		int BALL0_S_MIN = 0;
		int BALL0_S_MAX = 69;
		int BALL0_V_MIN = 124;
		int BALL0_V_MAX = 249;

		//Ball secondary color calibration variables
		int BALL1_H_MIN = 0;
		int BALL1_H_MAX = 189;
		int BALL1_S_MIN = 0;
		int BALL1_S_MAX = 141;
		int BALL1_V_MIN = 0;
		int BALL1_V_MAX = 95;

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

		cv::Mat preprocessBall(void);
		ballScreenParameters findBallThreshold(cv::Mat);
		ballScreenParameters findBallCountour(cv::Mat);
		//cv::vector<cv::Vec3f> findBallArea(cv::Mat);

		fieldScreenParameters findField(void);
		cv::Mat fillHoles(cv::Mat);

		void displayFeed(void);
		void detectGoal(void);
		void detectBall(void);

	public:
		void processFrame(void);

		ballScreenParameters getBallScreenParameters();
		goalScreenParameters getGoalScreenParameters();
		void setBallScreenParameters(ballScreenParameters);
		void setGoalScreenParameters(goalScreenParameters);

		Vision();
		virtual ~Vision(void);
};

#endif /* VISION_H_ */
