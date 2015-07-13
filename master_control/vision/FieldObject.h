/*
 * FieldObject.h
 *
 *  Created on: Jul 2, 2015
 *      Author: ryan
 */

#ifndef FIELDOBJECT_H_
#define FIELDOBJECT_H_

#include "pthread.h"
#include <opencv/cv.h>
#include <ctime>

//Grass color calibration variables
#define GRASS_H_MIN  44
#define GRASS_H_MAX  91
#define GRASS_S_MIN  83
#define GRASS_S_MAX  256
#define GRASS_V_MIN  0
#define GRASS_V_MAX  256


//Detector Variables
#define THRESH_ERODE_LIMIT 3
#define THRESH_DIALATE_LIMIT 2
 
#define ERODE_KERNAL_SIZE 2
#define DIALATE_KERNAL_SIZE 2
#define BLUR_KERNAL_SIZE 9 //Amount of blurring of original image (Notice: must be odd)


struct fieldScreenParameters {
	std::clock_t timeStamp;
	bool onScreen;
	cv::Rect boundries;
};

class FieldObject {
	private:

		//Locks
		//pthread_mutex_t physicalParametersLock;
		pthread_mutex_t screenParametersLock;

		//Main Variables
		//physicalParameters physicalBall;
		fieldScreenParameters screenField;

	private:
		cv::Mat fillHoles(cv::Mat);
		cv::Mat blurImage(cv::Mat);
		fieldScreenParameters findThreshold(cv::Mat);

	public:
		fieldScreenParameters getScreenParameters(void);
		void setScreenParameters(fieldScreenParameters);

		void detect(cv::Mat);

		FieldObject();
		virtual ~FieldObject();
};

#endif /* FIELDOBJECT_H_ */
