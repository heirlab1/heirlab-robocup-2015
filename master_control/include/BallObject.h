#ifndef BALLOBJECT_H_
#define BALLOBJECT_H_

#include "FieldObject.h"

#include <pthread.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <ctime>
#include <queue>

//Ball primary color calibration variables
#define BALL0_H_MIN  0
#define BALL0_H_MAX  134
#define BALL0_S_MIN  0
#define BALL0_S_MAX  69
#define BALL0_V_MIN  124
#define BALL0_V_MAX  249

//Ball secondary color calibration variables
#define BALL1_H_MIN  0
#define BALL1_H_MAX  189
#define BALL1_S_MIN  0
#define BALL1_S_MAX  141
#define BALL1_V_MIN  0
#define BALL1_V_MAX  95

//Grass color calibration variables
#define GRASS_H_MIN  44
#define GRASS_H_MAX  91
#define GRASS_S_MIN  83
#define GRASS_S_MAX  256
#define GRASS_V_MIN  0
#define GRASS_V_MAX  256


#define maxTime 0.9 // (1 = 1s, 0.5 = 500ms)

#define screenBallQueueMaxSize 3


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

		//Detector Tools
		cv::Mat erodeElement; //Erodes whitespaces to get rid of background noise
		cv::Mat dilateElement; //Dialates whitespaces to make bigger and fill voids
		std::queue<ballScreenParameters> screenRecord;

		//Locks
		pthread_mutex_t physicalParametersLock;
		pthread_mutex_t screenParametersLock;

		//Main Variables
		FieldObject* field;
		ballPhysicalParameters physicalBall;
		std::queue<ballScreenParameters> screenBallQueue;
		ballScreenParameters screenBall;

		//Debug tools
		cv::Mat imageDebug0, imageDebug1, imageDebug2;

	private:
		void displayDebug(void);

		ballScreenParameters findBallMode(void);

		cv::Mat fillHoles(cv::Mat);
		cv::Mat threshImage(cv::Mat);
		cv::Mat blurImage(cv::Mat);

		std::vector<cv::Vec3f> filtherOnField(cv::Mat, std::vector<cv::Vec3f>);
		std::vector<cv::Vec4f> filtherColorNearby(cv::Mat, std::vector<cv::Vec4f>);
		std::vector<cv::Vec4f> filtherColorAmount(cv::Mat, std::vector<cv::Vec4f>, float);

		ballScreenParameters findThreshold(cv::Mat);
		ballScreenParameters findContours(cv::Mat);
		cv::Rect findField(cv::Mat);

		bool checkElapsedTime(std::clock_t);

	public:
		ballPhysicalParameters getPhysicalParameters(void);
		ballScreenParameters getScreenParameters(void);
		void setPhysicalParameters(ballPhysicalParameters);
		void setScreenParameters(ballScreenParameters);

		ballScreenParameters detect(cv::Mat);

		void setPointers(FieldObject*);

		BallObject(void);
		virtual ~BallObject();
};

#endif /* BALLOBJECT_H_ */
