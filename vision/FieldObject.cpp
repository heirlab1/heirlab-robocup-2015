/*
 * FieldObject.cpp
 *
 *  Created on: Jul 2, 2015
 *      Author: ryan
 */

#include "FieldObject.h"

fieldScreenParameters FieldObject::getScreenParameters() {
	pthread_mutex_lock(&screenParametersLock);
	fieldScreenParameters tempField = screenField;
	pthread_mutex_unlock(&screenParametersLock);
	return tempField;
}

void FieldObject::setScreenParameters(fieldScreenParameters tempParameters) {
	pthread_mutex_lock(&screenParametersLock);
	screenField = tempParameters;
	pthread_mutex_unlock(&screenParametersLock);
}

void FieldObject::detect(cv::Mat imageHSV) {
	setScreenParameters(findThreshold(imageHSV));
}

// Fills empty holes of binary image (black voids)
cv::Mat FieldObject::fillHoles(cv::Mat imageTemp) {
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

//Finds field boundaries (rectangle)
fieldScreenParameters FieldObject::findThreshold(cv::Mat imageHSV) {
	imageHSV = blurImage(imageHSV);

	cv::Mat imageThreshold;
	cv::Rect fieldBoundries;
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;

	cv::inRange(imageHSV, cv::Scalar(GRASS_H_MIN, GRASS_S_MIN, GRASS_V_MIN), cv::Scalar(GRASS_H_MAX, GRASS_S_MAX, GRASS_V_MAX), imageThreshold); // Filter image according to grass thresholds
	imageThreshold = fillHoles(imageThreshold);

	imageHSV.release();

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
		fieldCanidate.onScreen = true;
		fieldCanidate.boundries = fieldBoundries;
		fieldCanidate.timeStamp = clock();
	}
	else {
		fieldCanidate.onScreen = false;
		fieldCanidate.timeStamp = clock();
	}

	return fieldCanidate;
}

//Blurs image
cv::Mat FieldObject::blurImage(cv::Mat tempImage) {
	// Blur Image
	if (BLUR_KERNAL_SIZE%2==0)
		cv::medianBlur(tempImage, tempImage, BLUR_KERNAL_SIZE+1);
	else
		cv::medianBlur(tempImage, tempImage, BLUR_KERNAL_SIZE);
	return tempImage;
}

FieldObject::FieldObject() {
}

FieldObject::~FieldObject() {
}

