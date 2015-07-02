/*
 * Vision.cpp
 *
 *  Created on: Jun 28, 2015
 *      Author: ryan
 */

#include "Vision.h"
//#include <opencv/highgui.h>
#include <opencv/cv.h>

ballScreenParameters Vision::getBallScreenParameters() {
	pthread_mutex_lock(&ballScreenParametersLock);
	ballScreenParameters tempBall = ball;
	pthread_mutex_unlock(&ballScreenParametersLock);
	return tempBall;
}

goalScreenParameters Vision::getGoalScreenParameters() {
	pthread_mutex_lock(&goalScreenParametersLock);
	ballScreenParameters tempGoal = goal;
	pthread_mutex_unlock(&goalScreenParametersLock);
	return tempGoal;
}

void Vision::setBallScreenParameters(ballScreenParameters tempBall) {
	pthread_mutex_lock(&ballScreenParametersLock);
	ball = tempBall;
	pthread_mutex_unlock(&ballScreenParametersLock);
}
void Vision::setGoalScreenParameters(goalScreenParameters tempGoal) {
	pthread_mutex_lock(&goalScreenParametersLock);
	goal = tempGoal;
	pthread_mutex_unlock(&goalScreenParametersLock);
}

void Vision::detectBall(void) {

	field = findField();

	cv::Mat imageBallThreshold = preprocessBall();
	ballScreenParameters ballCanidate = findBallThreshold(imageBallThreshold);
	if(ballCanidate.found) {
		if(field.boundries.x <= ballCanidate.x+ballCanidate.radius && ballCanidate.x-ballCanidate.radius <= field.boundries.x+field.boundries.width && field.boundries.y <= ballCanidate.y+ballCanidate.radius && ballCanidate.y-ballCanidate.radius <= field.boundries.y+field.boundries.height)
			setBallScreenParameters(ballCanidate);
	}
}

// Detects a ball via the threshold method (Takes in thresholded image, returns potential circle locations)
ballScreenParameters Vision::findBallThreshold(cv::Mat imageTemp) {

	cv::vector<cv::Vec3f> detectedCircles;
	cv::HoughCircles(imageTemp, detectedCircles, CV_HOUGH_GRADIENT, 2, imageThreshold.rows/8, 256/2, 256/2, 0, 150);

	int foundCircleIndex = 0;
	for(size_t i = 0; i<detectedCircles.size(); i++) {
		cv::Point circleCenter((int)(detectedCircles[i][0]), (int)(detectedCircles[i][1]));
		int circleRadius = (int)(detectedCircles[i][2]);
		cv::circle(imageCameraFeed, circleCenter, circleRadius, cv::Scalar(0, 255, 0), 5);

		if(detectedCircles[foundCircleIndex][2]<detectedCircles[i][2]) //Find circle with largest radius and put that in output
			foundCircleIndex = i;
	}

	ballScreenParameters ballCanidate;
	if(0<detectedCircles.size()) {
		ballCanidate.found = true;
		ballCanidate.x = (int)(detectedCircles[foundCircleIndex][0]);
		ballCanidate.y = (int)(detectedCircles[foundCircleIndex][1]);
		ballCanidate.radius = cvRound(detectedCircles[foundCircleIndex][2]);
	}
	else
		ballCanidate.found = false;

	//displayFeed(); //Uncomment to view image processes for this section of code
	return ballCanidate;
}

//Preprocesses image for main ball detection methods
cv::Mat Vision::preprocessBall() {
	cv::Mat threshold0, threshold1, output;
	cv::inRange(imageHSV, cv::Scalar(BALL0_H_MIN, BALL0_S_MIN, BALL0_V_MIN), cv::Scalar(BALL0_H_MAX, BALL0_S_MAX, BALL0_V_MAX), threshold0); // Filter image according to ball thresholds
	cv::inRange(imageHSV, cv::Scalar(BALL1_H_MIN, BALL1_S_MIN, BALL1_V_MIN), cv::Scalar(BALL1_H_MAX, BALL1_S_MAX, BALL1_V_MAX), threshold1); // Filter image according to ball thresholds
	cv::bitwise_or(threshold0, threshold1, output, 0); //Merge threshold images

	//Free up memory as soon as possible
	threshold0.release();
	threshold1.release();

	erodeElement = getStructuringElement(cv::MORPH_RECT, cv::Size(ERODE_KERNAL_SIZE, ERODE_KERNAL_SIZE)); //Dialate and Erode to rid background noise and make clearer
	dilateElement = getStructuringElement(cv::MORPH_RECT, cv::Size(DIALATE_KERNAL_SIZE, DIALATE_KERNAL_SIZE)); //^Also use rectangles because faster than circles

	//Erode and dialate set amount of times
	for(int i = 0; i<THRESH_ERODE_LIMIT; i++) {
		cv::erode(output, output, erodeElement);
	}
	for(int i = 0; i<THRESH_DIALATE_LIMIT; i++) {
		cv::dilate(output, output, dilateElement);
	}

	output = fillHoles(output); //Fills black voids

	return output;
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

//Finds field boundaries (rectangle)
fieldScreenParameters Vision::findField(void) {
	cv::Rect fieldBoundries;
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;

	cv::inRange(imageHSV, cv::Scalar(GRASS_H_MIN, GRASS_S_MIN, GRASS_V_MIN), cv::Scalar(GRASS_H_MAX, GRASS_S_MAX, GRASS_V_MAX), imageThreshold); // Filter image according to grass thresholds
	imageThreshold = fillHoles(imageThreshold);

	int largest_green_area = 0;
	cv::findContours(imageThreshold, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
	for (size_t i=0; i<contours.size(); i++) {
		cv::Rect brect = cv::boundingRect(contours[i]);
		if(brect.area() > largest_green_area) {
			fieldBoundries = cv::boundingRect(contours[i]);
			largest_green_area = brect.area();
		}
	}

	fieldScreenParameters fieldCanidate;
	if(0<contours.size()) {
		fieldCanidate.found =true;
		fieldCanidate.boundries=fieldBoundries;
	}
	else
		fieldCanidate.found = false;

	return fieldCanidate;
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

