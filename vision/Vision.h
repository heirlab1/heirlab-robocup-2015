/*
 * Vision.h
 *
 *  Created on: Jun 28, 2015
 *      Author: ryan
 */

#ifndef VISION_H_
#define VISION_H_

#include <opencv/cv.h>

class Vision {
	private:
		const int ERODE_KERNAL_SIZE = 2;
		const int DIALATE_KERNAL_SIZE = 2;
		const int BLUR_KERNAL_SIZE = 15; //Amount of blurring of original image (Notice: must be odd)

		//Detector Variables
		const int THRESH_ERODE_LIMIT = 3;
		const int THRESH_DIALATE_LIMIT = 2;

	private:
		bool checkElapsedTime(void);
		void resetElapsedTime(void);

	public:

		cv::Mat fillHoles(cv::Mat);

		Vision();
		virtual ~Vision(void);
};

#endif /* VISION_H_ */
