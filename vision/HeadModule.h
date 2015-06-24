/*
 * HeadModule.h
 *
 *  Created on: Jun 12, 2015
 *      Author: ryan
 */

#ifndef HEADMODULE_H_
#define HEADMODULE_H_

#include <opencv/highgui.h>
#include <opencv/cv.h>
#include <ctime>
#include <cstdio>
#include <unistd.h>

//#include VisionModule.h

/*
#define MODEL_NUMBER_L			0
#define MODEL_NUMBER_H			1
#define VERSION_OF_FIRMWARE 	2
#define ID						3
#define BAUD_RATE				4
#define RETURN_DELAY_TIME		5
#define CW_ANGLE_LIMIT_L		6
#define CW_ANGLE_LIMIT_H		7
#define CCW_ANGLE_LIMIT_L		8
#define CCW_ANGLE_LIMIT_H		9
#define HIGHEST_TEMP			11
#define LOWEST_VOLTAGE			12
#define HIGHEST_VOLTAGE			13
#define MAX_TORQUE_L			14
#define MAX_TORQUE_H			15
#define STATUS_RETURN_LEVEL		16
#define ALARM_LED				17
#define ALARM_SHUTDOWN			18
*/

//Instructions for dynamixel motors
//See http://support.robotis.com/en/product/dynamixel/mx_series/mx-64.htm
#define RETURN_DELAY_TIME		5
#define CW_ANGLE_LIMIT			6
#define CCW_ANGLE_LIMIT			8
#define STATUS_RETURN_LEVEL		16
#define TORQUE_ENABLE			24
#define LED_ENABLE				25
#define GOAL_POSITION			30
#define MOVING_SPEED			32
#define MAX_TORQUE				34
#define PRESENT_POSITION		36
#define PRESENT_SPEED			38
#define PRESENT_LOAD			40
#define PRESENT_VOLTAGE			42
#define IS_MOVING				46
#define GOAL_ACCELERATION		73

//Motor IDs
const int PAN_MOTOR_ID = 23;
const int TILT_MOTOR_ID = 24;

//Motor
#define DEGREES_PER_POSITION	0.08789
#define RADIANS_PER_DEGREE		0.01745
#define NUM_MOTOR_POSITIONS 	4095

//Tracking Limits
const int upperLimit = 40*(1/DEGREES_PER_POSITION); //How many degrees up from staring straight forward the head is allowed to tilt
const int lowerLimit = -2*upperLimit; //^In down direction (!!!Must be negative!!!)
const int rightLimit = 45*(1/DEGREES_PER_POSITION); //How many degrees right from staring straight forward the head is allowed to pan
const int leftLimit = -1*rightLimit; //^In left direction (!!!Must be negative!!!)
const int motorMatrixAccuracy = 1; //Is accuracy of search algorithms and resolution of freedom in turning the motors | Notice: 2=50%, 4=25% ...

//How much of the motor matrix the camera can see in any of the 4 directions when centered in on point
const int cameraTiltLimit = 70*(1/DEGREES_PER_POSITION)/2; //Up & down
const int cameraPanLimit = 50*(1/DEGREES_PER_POSITION)/2; //Right & left

const float threshold = 1;

//const int elapsedTimeDelayMills = 18;


struct comparePoints {
	bool operator()(const cv::Point &a, const cv::Point &b) {
	return (a.x==b.x && a.y==b.y);
	}
};

class HeadModule {
	private:
		//Tracking data
		int offsetPosHorizontal;
		int offsetPosVertical;
		cv::Point motorsPosition;

		std::clock_t lastRequest;

		float motorMatrix[(upperLimit-lowerLimit)][(rightLimit-leftLimit)]; //[rows][columns]

	public:
		HeadModule(void);
		virtual ~HeadModule(void);

		void initMoters(void);
		void resetMotorMatrix(void);
		void markSeenMotorMatrix(cv::Point);
		cv::Point BFSmotorMatrix(cv::Point);
		void scan(void);
		void motorsMoveTo(cv::Point);
		bool motorsCheckMoving(void);
		void motorMoveTo(int, int);
		void printCommResult(void);
		void motorIncrement(int, int);
		void motorsWaitForStop(void);

		cv::Point motorsReadPosition(void);
		bool checkWithinLimits(cv::Point);

	private:
		void printErrorCode(void);
		void printCommStatus(int);

		bool checkElapsedTime(void);
		void resetElapsedTime(void);

		int motorRead(int, int);
		void motorWrite(int, int, int);
};

#endif /* HEADMODULE_H_ */
