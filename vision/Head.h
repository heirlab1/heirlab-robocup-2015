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
#define DIRECTION_UP			0
#define DIRECTION_DOWN			1
#define DIRECTION_RIGHT			2
#define DIRECTION_LEFT			3

/*
#define DIRECTION_UP_RIGHT		4
#define DIRECTION_UP_LEFT		5
#define DIRECTION_DOWN_RIGHT	6
#define DIRECTION_DOWN_LEFT		7
*/

//Motor IDs
const int PAN_MOTOR_ID = 24;
const int TILT_MOTOR_ID = 23;

//Centers of tilt and pan (ADD MOTOR OFFSETS HERE IF ANY) (Offsets are in motor positions from motor position 0)
const int TILT_CENTER = NUM_MOTOR_POSITIONS/2-598;
const int PAN_CENTER = NUM_MOTOR_POSITIONS/2-220;

//Reverses the direction of pan/tilt
const bool TILT_REVERSE = true;
const bool PAN_REVERSE = true;

//Tracking Limits
#define UPPER_LIMIT_DEG 10 //How many degrees up from staring straight forward the head is allowed to tilt
#define LOWER_LIMIT_DEG -42 //^In down direction (!!!Must be negative!!!) !!!!!DO NOT HAVE LOWER THAN -45 FOR OLD HEAD!!!
#define RIGHT_LIMIT_DEG 48 //How many degrees right from staring straight forward the head is allowed to pan
#define LEFT_LIMIT_DEG	 -48 //^In left direction (!!!Must be negative!!!)

//const int motorMatrixAccuracy = 1; //Is accuracy of search algorithms and resolution of freedom in turning the motors | Notice: 2=50%, 4=25% ...

//How much of the motor matrix the camera can see in any of the 4 directions when centered in on point
#define CAMERA_TILT_SIGHT		70 //Up & down
#define CAMERA_PAN_SIGHT 	50 //Right & left


const int acceleration = 80; //acceleration speed of motors
const int defaultSpeed = 30;

const int TILT_MULTIPLIER = 1;
const int PAN_MULTIPLIER = 1;

//const int elapsedTimeDelayMills = 18;

class Head {
	private:
		int upperLimitPos;
		int lowerLimitPos;
		int rightLimitPos;
		int leftLimitPos;
public:
		USB2AXmotorManager motorManager;

		//float motorMatrix[rightLimitPos][upperLimitPos]; //[X][Y]

	public:
		Head(void);
		virtual ~Head(void);
		void initMoters(void);

		void resetMotorMatrix(void);
		//void markSeenMotorMatrix(cv::Point);
		//cv::Point BFSsearch(cv::Point);

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
