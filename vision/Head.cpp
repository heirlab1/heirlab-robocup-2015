/*
 * HeadModule.cpp
 *
 *  Created on: Jun 12, 2015
 *      Author: ryan
 */

#include "Head.h"
#include <iostream>

//Stops head
void Head::stopHead() {
	int tiltPosition = motorManager.getMotorPosition(TILT_MOTOR_ID);
    int panPosition = motorManager.getMotorPosition(PAN_MOTOR_ID);
    motorManager.setMotorPosition(TILT_MOTOR_ID, tiltPosition);
    motorManager.setMotorPosition(PAN_MOTOR_ID, panPosition);
}

//Moves head in desired direction. Assume users of this method will use stopHead()
void Head::moveHead(int direction) {
	switch(direction) {
		case DIRECTION_UP:
			motorManager.setMotorPosition(TILT_MOTOR_ID, upperLimitPos);
			break;
		case DIRECTION_DOWN:
			motorManager.setMotorPosition(TILT_MOTOR_ID, lowerLimitPos);
			break;
		case DIRECTION_RIGHT:
			motorManager.setMotorPosition(PAN_MOTOR_ID, rightLimitPos);
			break;
		case DIRECTION_LEFT:
			motorManager.setMotorPosition(PAN_MOTOR_ID, leftLimitPos);
			break;
		/*case DIRECTION_UP_LEFT:
			motorManager.setMotorPosition(TILT_MOTOR_ID, upperLimitPos);
			motorManager.setMotorPosition(PAN_MOTOR_ID, leftLimitPos);
			break;
		case DIRECTION_UP_RIGHT:
			motorManager.setMotorPosition(TILT_MOTOR_ID, upperLimitPos);
			motorManager.setMotorPosition(PAN_MOTOR_ID, rightLimitPos);
			break;
		case DIRECTION_DOWN_LEFT:
			motorManager.setMotorPosition(TILT_MOTOR_ID, lowerLimitPos);
			motorManager.setMotorPosition(PAN_MOTOR_ID, leftLimitPos);
			break;
		case DIRECTION_DOWN_RIGHT:
			motorManager.setMotorPosition(TILT_MOTOR_ID, lowerLimitPos);
			motorManager.setMotorPosition(PAN_MOTOR_ID, rightLimitPos);
			break;*/
		default:
			std::cout<<direction<<" is not valid direction!"<<std::endl;
	}
}

//Moves Head to specific position
void Head::moveHeadTo(int x, int y) {
	if (checkWithinLimits(x, y)) {
		motorManager.setMotorPosition(TILT_MOTOR_ID, y);
		motorManager.setMotorPosition(PAN_MOTOR_ID, x);
	}
	else
		std::cout<<"Can't moving Head to ("<<x<<", "<<y<<"). Out of Bounds!"<<std::endl;
}

void Head::setTiltAngle(float angle) {
	if(TILT_REVERSE)
		motorManager.setMotorPosition(TILT_MOTOR_ID, -angle/DEGREES_PER_POSITION+TILT_CENTER);
	else
		motorManager.setMotorPosition(TILT_MOTOR_ID, angle/DEGREES_PER_POSITION+TILT_CENTER);
}

void Head::setPanAngle(float angle) {
	//std::cout<<"Moving motor to pos: "<<(-angle/DEGREES_PER_POSITION+PAN_CENTER)<<std::endl;
	if(PAN_REVERSE)
		motorManager.setMotorPosition(PAN_MOTOR_ID, (-angle/DEGREES_PER_POSITION+PAN_CENTER));
	else
		motorManager.setMotorPosition(PAN_MOTOR_ID, (angle/DEGREES_PER_POSITION+PAN_CENTER));
}


//Returns Tilt angle (0 = straight ahead, + = up, - = down)
float Head::getTiltAngle() {
	if(TILT_REVERSE)
		return -(motorManager.getMotorPositionAccurate(TILT_MOTOR_ID)-TILT_CENTER)*DEGREES_PER_POSITION;
	else
		return (motorManager.getMotorPositionAccurate(TILT_MOTOR_ID)-TILT_CENTER)*DEGREES_PER_POSITION;
}

//Returns Tilt angle (0 = straight ahead, + = right, - = left)
float Head::getPanAngle() {
	if (PAN_REVERSE)
		return -(motorManager.getMotorPositionAccurate(PAN_MOTOR_ID)-PAN_CENTER)*DEGREES_PER_POSITION;
	else
		return (motorManager.getMotorPositionAccurate(PAN_MOTOR_ID)-PAN_CENTER)*DEGREES_PER_POSITION;

	/*int motorPosition = motorManager.getMotorPositionAccurate(PAN_MOTOR_ID);
			if(TILT_REVERSE)
				if(motorPosition<rightLimitPos-20 || leftLimitPos+20<motorPosition) {
					motorManager.markError();
					motorPosition = motorManager.getMotorPositionAccurate(PAN_MOTOR_ID);
					return -(motorPosition-PAN_CENTER)*DEGREES_PER_POSITION;
				}
			else if(motorPosition<rightLimitPos-20 || leftLimitPos+20<motorPosition) {
				motorManager.markError();
				motorPosition = motorManager.getMotorPositionAccurate(PAN_MOTOR_ID);
				return (motorPosition-PAN_CENTER)*DEGREES_PER_POSITION;
			}*/
}

//Returns true if given point is within the movement limits
bool Head::checkWithinLimits(int x, int y) {
	if(x >= rightLimitPos)
		return false;
	else if(x <= leftLimitPos)
		return false;
	else if(y >= upperLimitPos)
		return false;
	else if(y <= lowerLimitPos)
		return false;
	else
		return true;
}

//Sets limits for motors, accounts for an offset and a reverse direction
void Head::setLimits() {
	if(TILT_REVERSE) {
		upperLimitPos = -UPPER_LIMIT_DEG/DEGREES_PER_POSITION+TILT_CENTER;
		lowerLimitPos = -LOWER_LIMIT_DEG/DEGREES_PER_POSITION+TILT_CENTER;
	}
	else {
		upperLimitPos = UPPER_LIMIT_DEG/DEGREES_PER_POSITION+TILT_CENTER;
		lowerLimitPos = LOWER_LIMIT_DEG/DEGREES_PER_POSITION+TILT_CENTER;
	}
	if(PAN_REVERSE) {
		rightLimitPos = -RIGHT_LIMIT_DEG/DEGREES_PER_POSITION+PAN_CENTER;
		leftLimitPos = -LEFT_LIMIT_DEG/DEGREES_PER_POSITION+PAN_CENTER;
	}
	else {
		rightLimitPos = RIGHT_LIMIT_DEG/DEGREES_PER_POSITION+PAN_CENTER;
		leftLimitPos = LEFT_LIMIT_DEG/DEGREES_PER_POSITION+PAN_CENTER;
	}
}

//Initializes constant motor values
void Head::initMoters() {
	if(TILT_REVERSE)
		motorManager.setLimits(TILT_MOTOR_ID, upperLimitPos, lowerLimitPos);
	else
		motorManager.setLimits(TILT_MOTOR_ID, lowerLimitPos, upperLimitPos);
	if(PAN_REVERSE)
		motorManager.setLimits(PAN_MOTOR_ID, rightLimitPos, leftLimitPos);
	else
		motorManager.setLimits(PAN_MOTOR_ID, leftLimitPos, rightLimitPos);

	motorManager.setAcceleration(TILT_MOTOR_ID, acceleration);
	motorManager.setAcceleration(PAN_MOTOR_ID, acceleration);
	motorManager.setSpeed(TILT_MOTOR_ID, defaultSpeed);
	motorManager.setSpeed(PAN_MOTOR_ID, defaultSpeed);
}

//Constructor
Head::Head() {
	setLimits();
	initMoters();
}

//Destructor
Head::~Head() {

}
