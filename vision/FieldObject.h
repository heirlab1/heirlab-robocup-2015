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

struct fieldScreenParameters {
	std::clock_t timeStamp;
	bool onScreen;
	cv::Rect boundries;
};

class FieldObject {
	private:

		//Grass calibration variables
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

		//Locks
		//pthread_mutex_t physicalParametersLock;
		pthread_mutex_t screenParametersLock;

		//Main Variables
		//physicalParameters physicalBall;
		fieldScreenParameters screenField;

	private:
		cv::Mat fillHoles(cv::Mat);
		fieldScreenParameters findThreshold(cv::Mat);

	public:
		fieldScreenParameters getScreenParameters(void);
		void setScreenParameters(fieldScreenParameters);

		void detect(cv::Mat);

		FieldObject();
		virtual ~FieldObject();
};

#endif /* FIELDOBJECT_H_ */
