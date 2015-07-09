
#include "BallObject.h"
#include <cmath>
#include <ctime>

ballPhysicalParameters BallObject::getPhysicalParameters() {
	pthread_mutex_lock(&physicalParametersLock);
	ballPhysicalParameters tempBall = physicalBall;
	pthread_mutex_unlock(&physicalParametersLock);
	return tempBall;
}

void BallObject::setPhysicalParameters(ballPhysicalParameters tempParameters) {
	pthread_mutex_lock(&physicalParametersLock);
	physicalBall = tempParameters;
	pthread_mutex_unlock(&physicalParametersLock);
}

ballScreenParameters BallObject::getScreenParameters() {
	pthread_mutex_lock(&screenParametersLock);
	ballScreenParameters tempBall = screenBall;
	pthread_mutex_unlock(&screenParametersLock);
	return tempBall;
}

void BallObject::setScreenParameters(ballScreenParameters tempParameters) {
	pthread_mutex_lock(&screenParametersLock);
	screenBall = tempParameters;
	pthread_mutex_unlock(&screenParametersLock);
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
					putText(cameraFeed,"Tracking Object", Point(0,50),2,1,Scalar(0,255,0),2);
					//draw object location on screen
					drawObject(x,y,cameraFeed);}

			}else putText(cameraFeed,"TOO MUCH NOISE! ADJUST FILTER",Point(0,50),1,2,Scalar(0,0,255),2);
		}
	}
*/


void BallObject::detect(cv::Mat imageCameraFeed) {
	//fieldScreenParameters tempFieldScreen = field->getScreenParameters();
	std::clock_t time = clock();

	ballScreenParameters ballCanidate = findContours(imageCameraFeed);//findThreshold(imageHSV);
	if(ballCanidate.onScreen) {
		screenBallQueue.push(ballCanidate);
		//setScreenParameters( ballCanidate);
	}
	std::clock_t currentTime = clock();

	setScreenParameters(findBallMode());
	std::cout<<"Time: "<<(float)(currentTime-time)/CLOCKS_PER_SEC<<std::endl;
	std::cout<<"QueueSize: "<<screenBallQueue.size()<<std::endl;
	ballScreenParameters ballFound = getScreenParameters();
		if(ballFound.onScreen) {
			cv::Point circleCenter((int)(ballFound.x), (int)(ballFound.y));
			int circleRadius = (int)(ballFound.radius);
			cv::circle(imageDebug0, circleCenter, circleRadius, cv::Scalar(204, 0, 0), 2);
	}
	displayDebug();
}

//Reads position for motor and returns position
ballScreenParameters BallObject::findBallMode() {

	std::queue<ballScreenParameters> outputQueue;

	int screenBallQueueSize;

	screenBallQueueSize = screenBallQueue.size();
	for(int i=0; i<screenBallQueueSize; i++) {
		ballScreenParameters tempBall = screenBallQueue.front();
		if(!checkElapsedTime(tempBall.timeStamp) && screenBallQueueMaxSize-i>screenBallQueueSize)
			outputQueue.push(tempBall);
		screenBallQueue.pop();
	}

	screenBallQueue = outputQueue; //Reneter new queue

	screenBallQueueSize = outputQueue.size();

	ballScreenParameters ballFound;

	if(0<screenBallQueue.size()) {
		ballScreenParameters ballArray[screenBallQueueSize]; //Convert queue to array
		float ballRadiusArray[screenBallQueueSize]; //Convert queue to array (is for averaging the radiuses for the detected ball)
		int ballTimesCounted[screenBallQueueSize];

		for(int i=0; i<screenBallQueueSize; i++){
			ballRadiusArray[i]=0;
			ballTimesCounted[i]=0;
			ballArray[i] = outputQueue.front();
			outputQueue.pop();
		}

		int maximumScreenDistance = 12;

		for(int i=0; i<screenBallQueueSize; i++) {
			for(int j=0; j<screenBallQueueSize; j++) {
				float distance = std::sqrt((ballArray[i].x-ballArray[j].x)*(ballArray[i].x-ballArray[j].x)+(ballArray[i].y-ballArray[j].y)*(ballArray[i].y-ballArray[j].y));
				if(distance<=maximumScreenDistance) {
					ballRadiusArray[i]=ballRadiusArray[i]+ballArray[j].radius;
					ballTimesCounted[i]  = ballTimesCounted[i]+1;
				}
			}
		}

		int modeIndex = 0;

		//Get index of sample with largest amount of nearby samples
		for(int i=1; i<screenBallQueueSize; i++) {
			if(ballTimesCounted[modeIndex]<ballTimesCounted[i])
				modeIndex=i;
		}

		ballFound.onScreen = true;
		ballFound.x = ballArray[modeIndex].x;
		ballFound.y = ballArray[modeIndex].y;
		ballFound.radius = ballRadiusArray[modeIndex]/ballTimesCounted[modeIndex];
		ballFound.timeStamp = clock();
	}
	else
		ballFound.onScreen = false;

	return ballFound;
}

void BallObject::displayDebug() {
	cv::waitKey(10);
	cv::imshow("Origional",imageDebug0);
	//cv::imshow("Thresh0",imageDebug1);
	//cv::imshow("Thresh1",imageDebug2);
}


//Blurs image
cv::Mat BallObject::blurImage(cv::Mat tempImage) {
	// Blur Image
	if (BLUR_KERNAL_SIZE%2==0)
		cv::medianBlur(tempImage, tempImage, BLUR_KERNAL_SIZE+1);
	else
		cv::medianBlur(tempImage, tempImage, BLUR_KERNAL_SIZE);
	return tempImage;
}

// Thresholds HSV image to binary image
cv::Mat BallObject::threshImage(cv::Mat imageHSV) {
	cv::Mat threshold0, threshold1, imageThreshold;
	cv::inRange(imageHSV, cv::Scalar(BALL0_H_MIN, BALL0_S_MIN, BALL0_V_MIN), cv::Scalar(BALL0_H_MAX, BALL0_S_MAX, BALL0_V_MAX), threshold0); // Filter image according to ball thresholds
	//cv::inRange(imageHSV, cv::Scalar(BALL1_H_MIN, BALL1_S_MIN, BALL1_V_MIN), cv::Scalar(BALL1_H_MAX, BALL1_S_MAX, BALL1_V_MAX), threshold1); // Filter image according to ball thresholds
	//cv::bitwise_or(threshold0, threshold1, imageThreshold, 0); //Merge threshold images
	return threshold0;
}

// Fills empty holes of binary image (black voids)
cv::Mat BallObject::fillHoles(cv::Mat imageTemp) {
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

std::vector<cv::Vec3f> BallObject::filtherOnField(cv::Mat imageThreshsold ,std::vector<cv::Vec3f> circles) {
	cv::Rect fieldBoundries;
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
	std::vector<cv::Vec3f> output;

	//imageDebug1 = imageThreshold;
	int largestGreenArea = 0;

	//Find largest green area and assume is field
	cv::findContours(imageThreshsold, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
	for (size_t i=0; i<contours.size(); i++) {
		cv::Rect brect = cv::boundingRect(contours[i]);
		if(brect.area() > largestGreenArea) {
			fieldBoundries = cv::boundingRect(contours[i]);
			largestGreenArea = brect.area();
		}
	}

	//Copy circles that are on field into new array
	for(size_t i = 0; i<circles.size(); i++) {
		cv::Point circleCenter((int)(circles[i][0]), (int)(circles[i][1]));
		if(fieldBoundries.x<circleCenter.x && fieldBoundries.y<circleCenter.y && circleCenter.x<fieldBoundries.x+fieldBoundries.width && circleCenter.y<fieldBoundries.y+fieldBoundries.height) {
			output.resize(i+1);
			output[i][0] = circles[i][0];
			output[i][1] = circles[i][1];
			output[i][2] = circles[i][2];
		}
	}
	return output;
}

std::vector<cv::Vec4f> BallObject::filtherColorNearby(cv::Mat imageThreshold ,std::vector<cv::Vec4f> circles) {
	std::vector<cv::Vec4f> output;

	//Filther is really on field (green is outide the detected circle)
	for(int i=0; i<circles.size(); i++) {
		cv::Point circleCenter((int)(circles[i][0]), (int)(circles[i][1]));
		int outerCircleRadius = (int)(circles[i][2]+12);
		int innerCircleRadius = (int)(circles[i][2]+6);

		cv::Mat cropedImage;
		cv::Mat mask = cv::Mat::zeros(imageThreshold.rows, imageThreshold.cols, CV_8UC1);
		cv::circle(mask, circleCenter, outerCircleRadius, cv::Scalar(255,255,255), -1, 8, 0);
		imageThreshold.copyTo(cropedImage, mask); // copy values of imgThresh to dst if mask is > 0.
		cv::circle(cropedImage, circleCenter, innerCircleRadius, cv::Scalar(0,0,0), -1, 8, 0);

		int count = cv::countNonZero(cropedImage);
		float countCuttoff =  .38*(3.14159*std::pow(outerCircleRadius, 2)-3.14159*std::pow(innerCircleRadius, 2));
		//float simularity = std::abs(count-idealCount)/idealCount ;
		//std::cout<<count<<" | "<<idealCount<<" | "<<simularity<<std::endl;

		//imageDebug2 = cropedImage;

		if(countCuttoff<count) {
			output.resize(i+1);
			output[i][0] = circles[i][0];
			output[i][1] = circles[i][1];
			output[i][2] = circles[i][2];
			output[i][3] = circles[i][3];
		}
	}
	return output;
}

std::vector<cv::Vec4f> BallObject::filtherColorAmount(cv::Mat imageThreshold ,std::vector<cv::Vec3f> circles) {
	std::vector<cv::Vec4f> output;
	imageDebug2 = imageThreshold;

	//Filther is really on field (green is outide the detected circle)
	for(int i=0; i<circles.size(); i++) {
		cv::Point circleCenter((int)(circles[i][0]), (int)(circles[i][1]));
		int circleRadius = (int)(circles[i][2]);

		cv::Mat cropedImage;
		cv::Mat mask = cv::Mat::zeros(imageThreshold.rows, imageThreshold.cols, CV_8UC1);
		cv::circle(mask, circleCenter, circleRadius, cv::Scalar(255,255,255), -1, 8, 0);
		imageThreshold.copyTo(cropedImage, mask); // copy values of imgThresh to dst if mask is > 0.

		int count = cv::countNonZero(cropedImage);
		float countCuttoff = .4*3.14159*std::pow(circleRadius, 2);
		float countCuttoff2 = .9*3.14159*std::pow(circleRadius, 2);
		float idealCount = .75*3.14159*std::pow(circleRadius, 2);
		float simularity = std::abs(count-idealCount)/idealCount ;

		if(countCuttoff<count && count<countCuttoff2) {
			output.resize(i+1);
			output[i][0] = circles[i][0];
			output[i][1] = circles[i][1];
			output[i][2] = circles[i][2];
			output[i][3] = simularity;
		}
	}
	return output;
}



ballScreenParameters BallObject::findContours(cv::Mat imageCameraFeed) {
	imageDebug0 = imageCameraFeed;
	//imageCameraFeed = blurImage(imageCameraFeed);
	cv::vector<cv::Vec3f> detectedCircles;
	cv::Mat imageGray, imageThresholdBallWhite, imageThresholdField, imageHSV;
	cvtColor(imageCameraFeed, imageHSV, cv::COLOR_BGR2HSV);
	//cvtColor(imageCameraFeed, imageGray, cv::COLOR_BGR2GRAY);
	cv::inRange(imageHSV, cv::Scalar(BALL0_H_MIN, BALL0_S_MIN, BALL0_V_MIN), cv::Scalar(BALL0_H_MAX, BALL0_S_MAX, BALL0_V_MAX), imageThresholdBallWhite);
	cv::inRange(imageHSV, cv::Scalar(GRASS_H_MIN, GRASS_S_MIN, GRASS_V_MIN), cv::Scalar(GRASS_H_MAX, GRASS_S_MAX, GRASS_V_MAX), imageThresholdField);

	imageDebug1 = imageGray;
	//imageDebug2 = imageHSV;
	cv::HoughCircles(imageThresholdBallWhite, detectedCircles, CV_HOUGH_GRADIENT, 10, imageCameraFeed.rows/6, 180, 1, 14, 80);

//	imageDebug = imageThreshold;

	//Display debug
	for(size_t i = 0; i<detectedCircles.size(); i++) {
		cv::Point circleCenter((int)(detectedCircles[i][0]), (int)(detectedCircles[i][1]));
		int circleRadius = (int)(detectedCircles[i][2]);
		cv::circle(imageDebug0, circleCenter, circleRadius, cv::Scalar(204, 255, 255), 2);
	}

	//Filther is on field (cirlce center is inside main field area)
	detectedCircles = filtherOnField(imageThresholdField, detectedCircles);

	//Display debug
	for(size_t i = 0; i<detectedCircles.size(); i++) {
		cv::Point circleCenter((int)(detectedCircles[i][0]), (int)(detectedCircles[i][1]));
		int circleRadius = (int)(detectedCircles[i][2]);
		cv::circle(imageDebug0, circleCenter, circleRadius, cv::Scalar(102, 255, 255), 2);
	}

		//Filther by how much white is inside the circle
		cv::vector<cv::Vec4f> detectedCirclesFinal = filtherColorAmount(imageThresholdBallWhite, detectedCircles);

	//Filther how much green surrosunds the ball
		detectedCirclesFinal = filtherColorNearby(imageThresholdField ,detectedCirclesFinal);

		//Display debug
			for(size_t i = 0; i<detectedCircles.size(); i++) {
				cv::Point circleCenter((int)(detectedCircles[i][0]), (int)(detectedCircles[i][1]));
				int circleRadius = (int)(detectedCircles[i][2]);
				cv::circle(imageDebug0, circleCenter, circleRadius, cv::Scalar(0, 128, 255), 2);
			}

		//Display debug
		for(size_t i = 0; i<detectedCirclesFinal.size(); i++) {
			cv::Point circleCenter((int)(detectedCirclesFinal[i][0]), (int)(detectedCirclesFinal[i][1]));
			int circleRadius = (int)(detectedCirclesFinal[i][2]);
			cv::circle(imageDebug0, circleCenter, circleRadius, cv::Scalar(126, 0, 255), 2);
		}

		ballScreenParameters ballCanidate;
		int foundCircleIndex = 0;

		if (0<detectedCirclesFinal.size()) {
			for(int i = 0; i<detectedCirclesFinal.size(); i++)  {
				if(detectedCirclesFinal[foundCircleIndex][3]<detectedCirclesFinal[i][3])
					foundCircleIndex = i;
			}

			cv::Point circleCenter((int)(detectedCircles[foundCircleIndex][0]), (int)(detectedCircles[foundCircleIndex][1]));
			int circleRadius = (int)(detectedCircles[foundCircleIndex][2]);
			cv::circle(imageDebug0, circleCenter, circleRadius, cv::Scalar(0, 0, 255), 2);

			ballCanidate.onScreen = true;
			ballCanidate.x = (int)(detectedCircles[foundCircleIndex][0]);
			ballCanidate.y = (int)(detectedCircles[foundCircleIndex][1]);
			ballCanidate.radius = cvRound(detectedCircles[foundCircleIndex][2]);
			ballCanidate.timeStamp = clock();
	}
	else
		ballCanidate.onScreen = false;

	//displayFeed(); //Uncomment to view image processes for this section of code
	return ballCanidate;

}


// Detects a ball via the threshold method (Takes in thresholded image, returns potential circle locations)
ballScreenParameters BallObject::findThreshold(cv::Mat imageHSV) {
	imageHSV = blurImage(imageHSV);
	cv::Mat imageThreshold = threshImage(imageHSV);

	//Erode and dialate set amount of times
	for(int i = 0; i<THRESH_ERODE_LIMIT; i++) {
		cv::erode(imageThreshold, imageThreshold, erodeElement);
	}
	for(int i = 0; i<THRESH_DIALATE_LIMIT; i++) {
		cv::dilate(imageThreshold, imageThreshold, dilateElement);
	}

	//imageThreshold = fillHoles(imageThreshold); //Fills black voids

	//imageDebug0 = imageHSV;

	cv::vector<cv::Vec3f> detectedCircles;
	cv::HoughCircles(imageThreshold, detectedCircles, CV_HOUGH_GRADIENT, 2, imageThreshold.rows/4, 200, 10, 100, 200);

	int foundCircleIndex = 0;
	for(size_t i = 0; i<detectedCircles.size(); i++) {
		cv::Point circleCenter((int)(detectedCircles[i][0]), (int)(detectedCircles[i][1]));
		int circleRadius = (int)(detectedCircles[i][2]);
		cv::circle(imageDebug0, circleCenter, circleRadius, cv::Scalar(255, 0, 0), 5);

		if(detectedCircles[foundCircleIndex][2]<detectedCircles[i][2]) //Find circle with largest radius and put that in output
			foundCircleIndex = i;
	}

	ballScreenParameters ballCanidate;
	if(0<detectedCircles.size()) {
		ballCanidate.onScreen = true;
		ballCanidate.x = (int)(detectedCircles[foundCircleIndex][0]);
		ballCanidate.y = (int)(detectedCircles[foundCircleIndex][1]);
		ballCanidate.radius = cvRound(detectedCircles[foundCircleIndex][2]);
	}
	else
		ballCanidate.onScreen = false;

	//displayFeed(); //Uncomment to view image processes for this section of code
	return ballCanidate;
}

//Checks to see if time since last request has passed
bool BallObject::checkElapsedTime(std::clock_t time) {
	std::clock_t currentTime = clock();
	if (float(currentTime-time)/CLOCKS_PER_SEC > maxTime)
		return true;
	else
		return false;
}

void BallObject::setPointers(FieldObject* tempField) {
	field = tempField;
}

BallObject::BallObject() {
	std::cout<<"Created ball object"<<std::endl;
}

BallObject::~BallObject() {
}

