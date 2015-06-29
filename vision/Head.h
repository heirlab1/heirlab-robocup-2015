/*
 * HeadModule.h
 *
 *  Created on: Jun 12, 2015
 *      Author: ryan
 */

#ifndef HEAD_H_
#define HEAD_H_

#include "USB2AXmotorManager.h"

//Head Directions
#define MOVE_HEAD_UP		0
#define MOVE_HEAD_DOWN		1
#define MOVE_HEAD_RIGHT		2
#define MOVE_HEAD_LEFT		3

//Motor IDs
const int PAN_MOTOR_ID = 23;
const int TILT_MOTOR_ID = 24;

//Centers of tilt and pan (ADD MOTOR OFFSETS HERE IF ANY) (Offsets are in motor positions from motor position 0)
const int TILT_CENTER = NUM_MOTOR_POSITIONS/2+100;
const int PAN_CENTER = NUM_MOTOR_POSITIONS/2;

//Reverses the direction of pan/tilt
const bool TILT_REVERSE = true;
const bool PAN_REVERSE = false;

//Tracking Limits
const int upperLimitDeg = 10; //How many degrees up from staring straight forward the head is allowed to tilt
const int lowerLimitDeg = -40; //^In down direction (!!!Must be negative!!!) !!!!!DO NOT HAVE LOWER THAN -45 FOR OLD HEAD!!!
const int rightLimitDeg = 48; //How many degrees right from staring straight forward the head is allowed to pan
const int leftLimitDeg = -48; //^In left direction (!!!Must be negative!!!)

//const int motorMatrixAccuracy = 1; //Is accuracy of search algorithms and resolution of freedom in turning the motors | Notice: 2=50%, 4=25% ...

//How much of the motor matrix the camera can see in any of the 4 directions when centered in on point
const int cameraTiltLimit = 70; //Up & down
const int cameraPanLimit = 50; //Right & left


const int acceleration = 100; //acceleration speed of motors
const int defaultSpeed = 60;

const int TILT_MULTIPLIER = 1;;
const int PAN_MULTIPLIER = 1;

//const int elapsedTimeDelayMills = 18;

class Head {
	private:
		int upperLimitPos;
		int lowerLimitPos;
		int rightLimitPos;
		int leftLimitPos;

		USB2AXmotorManager motorManager;

		//float motorMatrix[rightLimitPos][upperLimitPos]; //[X][Y]

	public:
		Head(void);
		virtual ~Head(void);
		void initMoters(void);

		void resetMotorMatrix(void);
		//void markSeenMotorMatrix(cv::Point);
		//cv::Point BFSmotorMatrix(cv::Point);

		void scan(void);
		void stopHead(void);
		void moveHead(int);
		void moveHeadTo(int, int);

		void setTiltAngle(float);
		void setPanAngle(float);

		float getTiltAngle(void);
		float getPanAngle(void);

		bool checkWithinLimits(int, int);
private:
		void setLimits(void);

};

#endif /* HEADMODULE_H_ */
