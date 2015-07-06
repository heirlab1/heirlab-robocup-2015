/*
 * Camera.h
 *
 *  Created on: Jul 2, 2015
 *      Author: ryan
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include <ctime>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#define FRAME_WIDTH		640
#define FRAME_HEIGHT	480

class Camera {
	private:
		cv::Mat imageCameraFeed; //Direct camera image
		cv::Mat imageHSV; //HSV image

		cv::VideoCapture capture; //Video capture object required for acquiring videocamra feed
		float minTime = 0.05; //Min time between capturing new image from capturing new image and processing (1 = 1s, 0.5 = 500ms)
		std::clock_t lastRequest;

		void setupCamera(void);
		bool checkElapsedTime(void);
		void resetElapsedTime(void);
		void captureFrame(void);

	public:
		cv::Mat getCameraImage(void);
		cv::Mat getHSVImage(void);

		Camera();
		virtual ~Camera();
};

#endif /* CAMERA_H_ */
