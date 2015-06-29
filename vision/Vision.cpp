/*
 * Vision.cpp
 *
 *  Created on: Jun 28, 2015
 *      Author: ryan
 */

#include "Vision.h"
//#include <opencv/highgui.h>
#include <opencv/cv.h>

// Detects a ball via the threshold method
cv::Point Vision::findBallThreshold() {
	bool foundBall = false; //Ball assumed not found

	cv::inRange(imageHSV, cv::Scalar(BALL_H_MIN, BALL_S_MIN, BALL_V_MIN), cv::Scalar(BALL_H_MAX, BALL_S_MAX, BALL_V_MAX), imageThreshold); // Filter image according to ball thresholds
	erodeElement = getStructuringElement(cv::MORPH_RECT, cv::Size(ERODE_KERNAL_SIZE, ERODE_KERNAL_SIZE)); //Dialate and Erode to rid background noise and make clearer
	dilateElement = getStructuringElement(cv::MORPH_RECT, cv::Size(DIALATE_KERNAL_SIZE, DIALATE_KERNAL_SIZE)); //^Also use rectangles because faster than circles
	imageThreshold  = fillHoles(imageThreshold);


	//Erode and dialate set amount of times
	for(int i = 0; i<THRESH_ERODE_LIMIT; i++) {
		cv::erode(imageThreshold, imageThreshold, erodeElement);
	}
	for(int i = 0; i<THRESH_DIALATE_LIMIT; i++) {
		cv::dilate(imageThreshold, imageThreshold, dilateElement);
	}

	cv::vector<cv::Vec3f> detectedCircles;
	cv::HoughCircles(imageThreshold, detectedCircles, CV_HOUGH_GRADIENT, 2, imageThreshold.rows/8, 100, 50, 0, 150);

	int foundCircleIndex = 0;
	for(size_t i = 0; i<detectedCircles.size(); i++) {
		foundBall=true;
		cv::Point circleCenter(cvRound(detectedCircles[i][0]), cvRound(detectedCircles[i][1]));
		int circleRadius = cvRound(detectedCircles[i][2]);
		cv::circle(imageCameraFeed, circleCenter, circleRadius, cv::Scalar(0, 255, 0), 5);

		if(detectedCircles[foundCircleIndex][2]<detectedCircles[i][2]) { //Find circle with largest radius and put that in output
			foundCircleIndex = i;
		}
	}

	cv::Point ball;
	if(foundBall) {
		int circleRadius = cvRound(detectedCircles[foundCircleIndex][2]);
		ball = cv::Point(detectedCircles[foundCircleIndex][0], detectedCircles[foundCircleIndex][1]);
		//cv::putText(imageCameraFeed, cv::Point(detectedCircles[foundCircleIndex][0], detectedCircles[foundCircleIndex][0]+50),2,1, cv::Scalar(0,255,0),2);

		//std::string output;
		//output<<ball.x<<" "<<ball.y;


		//cv::putText(imageCameraFeed, output, cv::Point(0,50),1,2,cv::Scalar(0,0,255),2);
	}
	else {
		//cv::putText(imageCameraFeed, "No ball found", cv::Point(0,50),1,2,cv::Scalar(0,0,255),2);
		ball = cv::Point(-1,-1);
	}
	//std::cout<<"Ball location: "<<ball<<std::endl;
	displayFeed(); //Uncomment to view image processes for this section of code
	return ball;
}

/*
void Vision::detectBallArea() {
	void trackFilteredObject(int &x, int &y, Mat threshold, Mat &cameraFeed){

		Mat temp;
		threshold.copyTo(temp);
		//these two vectors needed for output of findContours
		vector< vector<Point> > contours;
		vector<Vec4i> hierarchy;
		//find contours of filtered image using openCV findContours function
		findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );
		//use moments method to find our filtered object
		double refArea = 0;
		bool objectFound = false;
		if (hierarchy.size() > 0) {
			int numObjects = hierarchy.size();
	        //if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
	        if(numObjects<MAX_NUM_OBJECTS){
				for (int index = 0; index >= 0; index = hierarchy[index][0]) {

					Moments moment = moments((cv::Mat)contours[index]);
					double area = moment.m00;

					//if the area is less than 20 px by 20px then it is probably just noise
					//if the area is the same as the 3/2 of the image size, probably just a bad filter
					//we only want the object with the largest area so we safe a reference area each
					//iteration and compare it to the area in the next iteration.
	                if(area>MIN_OBJECT_AREA && area<MAX_OBJECT_AREA && area>refArea){
						x = moment.m10/area;
						y = moment.m01/area;
						objectFound = true;
						refArea = area;
					}else objectFound = false;


				}
				//let user know you found an object
				if(objectFound ==true){
					putText(cameraFeed,"Tracking Object",Point(0,50),2,1,Scalar(0,255,0),2);
					//draw object location on screen
					drawObject(x,y,cameraFeed);}

			}else putText(cameraFeed,"TOO MUCH NOISE! ADJUST FILTER",Point(0,50),1,2,Scalar(0,0,255),2);
		}
	}
*/

//Finds area of field on screen
cv::Rect Vision::findField(void) {

	cv::Rect fieldBoundries;

	cv::inRange(imageHSV, cv::Scalar(GRASS_H_MIN, GRASS_S_MIN, GRASS_V_MIN), cv::Scalar(GRASS_H_MAX, GRASS_S_MAX, GRASS_V_MAX), imageThreshold); // Filter image according to grass thresholds

	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;

	int largest_green_area = 0;
	cv::findContours(imageThreshold, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
	for ( size_t i=0; i<contours.size(); i++) {
		cv::Rect brect = cv::boundingRect(contours[i]);
		if(brect.area() > largest_green_area) {
			fieldBoundries = cv::boundingRect(contours[i]);
			largest_green_area = brect.area();
		}
	}

	return fieldBoundries;
}

// Fills empty holes of binary image (black voids)
cv::Mat Vision::fillHoles(cv::Mat imageTemp) {
//From: http://answers.opencv.org/question/9863/fill-holes-of-a-binary-image/

	for (int i = 0; i < imageTemp.cols; i++) {
	    if (imageTemp.at<char>(0, i) == 0)
	        cv::floodFill(imageTemp, cv::Point(i, 0), 255, 0, 10, 10);
	    if (imageTemp.at<char>(imageTemp.rows-1, i) == 0)
	        cv::floodFill(imageTemp, cv::Point(i, imageTemp.rows-1), 255, 0, 10, 10);
	}
	for (int i = 0; i < imageTemp.rows; i++) {
	    if (imageTemp.at<char>(i, 0) == 0)
	        cv::floodFill(imageTemp, cv::Point(0, i), 255, 0, 10, 10);
	    if (imageTemp.at<char>(i, imageTemp.cols-1) == 0)
	        cv::floodFill(imageTemp, cv::Point(imageTemp.cols-1, i), 255, 0, 10, 10);
	}
	return imageTemp;
}

//Processes frame for goal detection and ball detection
void Vision::preprocessFrame() {
	capture.read(imageCameraFeed); //Capture new image

	// Blur Image
	if (BLUR_KERNAL_SIZE%2==0)
				cv::medianBlur(imageCameraFeed, imageCameraFeed, BLUR_KERNAL_SIZE+1);
			else
				cv::medianBlur(imageCameraFeed, imageCameraFeed, BLUR_KERNAL_SIZE);
	cv::cvtColor(imageCameraFeed, imageHSV, cv::COLOR_BGR2HSV); //convert BGR(RGB) to HSV
}

//Setusp Frame
void Vision::setupFrame() {
	capture.open(0);

	//set height and width of capture frame
	capture.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);
	cv::waitKey(30);
}

//Checks to see if time since last request has passed
bool Vision::checkElapsedTime() {
	std::clock_t currentTime = clock();
	if (float(currentTime-lastRequest)/CLOCKS_PER_SEC > minTime)
		return true;
	else
		return false;
}

//Resets time since last request to current time
void Vision::resetElapsedTime() {
	lastRequest = clock();
}

Vision::Vision() {
	setupFrame();
}

Vision::~Vision() {
	capture.release();
}

