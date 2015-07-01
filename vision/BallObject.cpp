/*

 * BallObject.cpp
 *
 *  Created on: Jul 1, 2015
 *      Author: ryan


#include "BallObject.h"



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
screenParameters Vision::findBallThreshold(cv::Mat imageTemp) {

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

	screenParameters ballCanidate;
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

cv::Mat BallObject::ProcessImage() {

}





//Preprocesses image for main ball detection methods
cv::Mat Vision::preprocessBall() {
	cv::Mat threshold0, threshold1, output;
	cv::inRange(imageHSV, cv::Scalar(BALL0_H_MIN, BALL0_S_MIN, BALL0_V_MIN), cv::Scalar(BALL0_H_MAX, BALL0_S_MAX, BALL0_V_MAX), threshold0); // Filter image according to ball thresholds
	cv::inRange(imageHSV, cv::Scalar(BALL1_H_MIN, BALL1_S_MIN, BALL1_V_MIN), cv::Scalar(BALL1_H_MAX, BALL1_S_MAX, BALL1_V_MAX), threshold1); // Filter image according to ball thresholds
	cv::bitwise_or(threshold0, threshold1, output, 0); //Merge threshold images

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

BallObject::BallObject() {
	// TODO Auto-generated constructor stub

}

BallObject::~BallObject() {
	// TODO Auto-generated destructor stub
}
*/

