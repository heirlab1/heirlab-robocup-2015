#ifndef BALLOBJECT_H_
#define BALLOBJECT_H_

#include "FieldObject.h"

#include <pthread.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <ctime>
#include <queue>

struct ballPhysicalParameters {
	std::clock_t timeStamp;
	float distance;
	float angle;
};

struct ballScreenParameters {
	std::clock_t timeStamp;
	bool onScreen;
	int x, y, radius;
};

class BallObject {
	private:
		//Ball primary color calibration variables
		const int BALL0_H_MIN = 0;
		const int BALL0_H_MAX = 134;
		const int BALL0_S_MIN = 0;
		const int BALL0_S_MAX = 69;
		const int BALL0_V_MIN = 124;
		const int BALL0_V_MAX = 249;

		//Ball secondary color calibration variables
		const int BALL1_H_MIN = 0;
		const int BALL1_H_MAX = 189;
		const int BALL1_S_MIN = 0;
		const int BALL1_S_MAX = 141;
		const int BALL1_V_MIN = 0;
		const int BALL1_V_MAX = 95;

		const int GRASS_H_MIN = 34;
		const int GRASS_H_MAX = 86;
		const int GRASS_S_MIN = 125;
		const int GRASS_S_MAX = 256;
		const int GRASS_V_MIN = 37;
		const int GRASS_V_MAX = 256;

		//Detector Variables
		const int THRESH_ERODE_LIMIT = 3;
		const int THRESH_DIALATE_LIMIT = 2;

		const int ERODE_KERNAL_SIZE = 2;
		const int DIALATE_KERNAL_SIZE = 2;
		const int BLUR_KERNAL_SIZE = 9; //Amount of blurring of original image (Notice: must be odd)

		//Detector Tools
		cv::Mat erodeElement; //Erodes whitespaces to get rid of background noise
		cv::Mat dilateElement; //Dialates whitespaces to make bigger and fill voids
		std::queue<ballScreenParameters> screenRecord;

		float maxTime = 5; //Min time between dynamixel communication bursts (1 = 1s, 0.5 = 500ms)

		//Locks
		pthread_mutex_t physicalParametersLock;
		pthread_mutex_t screenParametersLock;

		//Main Variables
		FieldObject* field;
		ballPhysicalParameters physicalBall;
		ballScreenParameters screenBall;

		//Debug tools
		cv::Mat imageDebug0, imageDebug1, imageDebug2;

	private:
		void displayDebug(void);

		cv::Mat fillHoles(cv::Mat);
		cv::Mat threshImage(cv::Mat);
		cv::Mat blurImage(cv::Mat);

		std::vector<cv::Vec3f> filtherOnField(cv::Mat, std::vector<cv::Vec3f>);
		std::vector<cv::Vec3f> filtherGreenNearby(cv::Mat, std::vector<cv::Vec3f>);
		std::vector<cv::Vec4f> filtherWhiteness(cv::Mat, std::vector<cv::Vec3f>);

		ballScreenParameters findThreshold(cv::Mat);
		ballScreenParameters findContours(cv::Mat);
		cv::Rect findField(cv::Mat);

		bool checkElapsedTime(std::clock_t);

	public:
		ballPhysicalParameters getPhysicalParameters(void);
		ballScreenParameters getScreenParameters(void);
		void setPhysicalParameters(ballPhysicalParameters);
		void setScreenParameters(ballScreenParameters);

		void detect(cv::Mat);

		void setPointers(FieldObject*);

		BallObject(void);
		virtual ~BallObject();
};

#endif /* BALLOBJECT_H_ */
