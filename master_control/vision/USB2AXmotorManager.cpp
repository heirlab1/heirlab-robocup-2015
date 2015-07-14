#include "USB2AXmotorManager.h"
#include <dynamixel.h>
#include "unistd.h"
#include <set>
#include <cmath>
#include <iostream>

//Reads position for motor and returns position
int USB2AXmotorManager::getMotorPosition(int motorID) {
	return readMotor(motorID, PRESENT_POSITION);
	//return motorsPosition;
}

//Reads position for motor and returns position
int USB2AXmotorManager::getMotorPositionAccurate(int motorID) {
	int accuracy = 15;
	int sampleSize = 6;
	int position[sampleSize];
	int times[sampleSize];
	int modeIndex = 0;

	//Take N motor position samples
	for(int i=0; i<sampleSize; i++) {
		position[i] = readMotor(motorID, PRESENT_POSITION);
		if (position[i] == NULL) {
			i--;
			resetElapsedTime();
			while(!checkElapsedTime());
		}
		times[i] = 0;
		//usleep(1000*250); //Wait 250ms as motors don't like being read from too often
	}

	//Record how many samples are near given sample
	for(int i=0; i<sampleSize; i++) {
		for(int j=0; j<sampleSize; j++) {
			if(std::abs(position[i]-position[j])< accuracy)
				times[i] = times[i]+1;
		}
	}

	//Get index of sample with largest amount of nearby samples
	for(int i=1; i<sampleSize; i++) {
		if(times[modeIndex]<times[i])
			modeIndex=times[i];
	}

	return position[modeIndex];
}

//Moves selected motor to position
void USB2AXmotorManager::setMotorPosition(int motorID, int position) {
	writeMotor(motorID, GOAL_POSITION, position);
}
//Reads desired command from selected motor
//Is private function for error handling
int USB2AXmotorManager::readMotor(int motorID, int command) {
	while(!checkElapsedTime());
	int returnVal = dxl_read_word(motorID, command);
	//std::cout<<"ID: "<<motorID<<", CMD: "<<command<<", RTN: "<<returnVal<<std::endl;
	if (dxl_get_result() == COMM_RXCORRUPT ||
			(motorID == 23 && (command==36 && (returnVal==0 || returnVal==1536 || returnVal ==1280))) || //BlackList
			(motorID == 24 && (command==36 && (returnVal==0 || returnVal==1792 || returnVal == 1280))) //BlackList
			) {
		resetElapsedTime();
		while(!checkElapsedTime());
		disconnect();
		resetElapsedTime();
		while(!checkElapsedTime());
		connect();
		resetElapsedTime();
		while(!checkElapsedTime());
		returnVal = 0;
	}
	resetElapsedTime();
	//printCommResult();
	//std::cout<<"ID: "<<motorID<<", CMD: "<<command<<", RTN: "<<returnVal<<std::endl;
	return returnVal;
}

//Writes desired command to selected motor
//Is private function for error handling
void USB2AXmotorManager::writeMotor(int motorID , int command, int value) {
	while(!checkElapsedTime());
	//std::cout<<"ID: "<<motorID<<", CMD: "<<command<<", SND: "<<value<<std::endl;
	dxl_write_word(motorID, command, value);
	resetElapsedTime();
	//printCommResult();
	//std::cout<<command<<std::endl;
}

/*void Head::waitForStop() {
	int maxCounter = 5;
	int count = 0;
	while(count<maxCounter) {
		usleep(1000*250);
		if(!checkMoving())
			count++;
		else
			count=0;
	}
}*/

//Check if motors are moving
bool USB2AXmotorManager::checkMoving(int motorID) {
	if(readMotor(motorID, IS_MOVING))
		return true;
	else
		return false;
}

/*
void USB2AXmotorManager::incrementMotor(int id, int amount) {
	if(id==PAN_MOTOR_ID) {
		if(checkWithinLimits(cv::Point(motorPositions.x+amount, motorPositions.y))) {
			motorPositions=cv::Point(motorPositions.x+amount, motorPositions.y);
			writeMotor(PAN_MOTOR_ID, GOAL_POSITION, motorPositions.x);
		}
		else
			std::cout<<"Cannot increment PAN motor there!"<<std::endl;

	}
	else if(id ==TILT_MOTOR_ID) {
		if(checkWithinLimits(cv::Point(motorPositions.x, motorPositions.y+amount))) {
		motorPositions=cv::Point(motorPositions.x, motorPositions.y+amount);
		writeMotor(TILT_MOTOR_ID, GOAL_POSITION, motorPositions.y);
		}
		else
			std::cout<<"Cannot increment TILT motor there!"<<std::endl;
	}
}
*/

//Checks to see if time since last request has passed
bool USB2AXmotorManager::checkElapsedTime() {
	std::clock_t currentTime = clock();
	if (float(currentTime-lastRequest)/CLOCKS_PER_SEC > minTime)
		return true;
	else
		return false;
}

//Resets time since last request to current time
void USB2AXmotorManager::resetElapsedTime() {
	lastRequest = clock();
}

//Finds Errors in transmission and prints
void USB2AXmotorManager::printCommResult() {
	int commStatus = dxl_get_result();
	if (commStatus == COMM_RXSUCCESS)
		printErrorCode();
	else
		printCommStatus(commStatus);
}

//Prints error code
void USB2AXmotorManager::printErrorCode() {
	if(dxl_get_rxpacket_error(ERRBIT_VOLTAGE) == 1)
		std::cout<<"Input voltage error!"<<std::endl;
	if(dxl_get_rxpacket_error(ERRBIT_ANGLE) == 1)
		std::cout<<"Angle limit error!"<<std::endl;
	if(dxl_get_rxpacket_error(ERRBIT_OVERHEAT) == 1)
		std::cout<<"Overheat error!"<<std::endl;
	if(dxl_get_rxpacket_error(ERRBIT_RANGE) == 1)
		std::cout<<"Out of range error!"<<std::endl;
	if(dxl_get_rxpacket_error(ERRBIT_CHECKSUM) == 1)
		std::cout<<"Checksum error!"<<std::endl;
	if(dxl_get_rxpacket_error(ERRBIT_OVERLOAD) == 1)
		std::cout<<"Overload error!"<<std::endl;
	if(dxl_get_rxpacket_error(ERRBIT_INSTRUCTION) == 1)
		std::cout<<"Instruction code error!"<<std::endl;
}

//Prints COMM staus
void USB2AXmotorManager::printCommStatus(int commStatus) {
	switch(commStatus) {
		case COMM_TXFAIL:
			std::cout<<"COMM_TXFAIL: Failed transmit instruction packet!"<<std::endl;
			break;
		case COMM_TXERROR:
			std::cout<<"COMM_TXERROR: Incorrect instruction packet!"<<std::endl;
			break;
		case COMM_RXFAIL:
			std::cout<<"COMM_RXFAIL: Failed get status packet from device!"<<std::endl;
			break;
		case COMM_RXWAITING:
			std::cout<<"COMM_RXWAITING: Now receiving status packet!"<<std::endl;
			break;
		case COMM_RXTIMEOUT:
			std::cout<<"COMM_RXTIMEOUT: There is no status packet!"<<std::endl;
			break;
		case COMM_RXCORRUPT:
			std::cout<<"COMM_RXCORRUPT: Incorrect status packet!"<<std::endl;
			break;
		default:
			std::cout<<"This is unknown error code!"<<std::endl;
			break;
	}
}

/*void USB2AXmotorManager::markError() {
	errorCount++;
	std::cout<<"Motor Read Error"<<std::endl;
	if(errorLimit<errorCount) {
		disconnect();
		connect();
	}
}*/

void USB2AXmotorManager::setTorque(int motorID, bool state) {
	if(state)
		writeMotor(motorID, TORQUE_ENABLE, 1);
	else if(!state)
		writeMotor(motorID, TORQUE_ENABLE, 0);
}

void USB2AXmotorManager::setSpeed(int motorID, int speed) {
	writeMotor(motorID, MOVING_SPEED, speed);
}

void USB2AXmotorManager::setAcceleration(int motorID, int acceleration) {
	writeMotor(motorID, GOAL_ACCELERATION, acceleration);
}

void USB2AXmotorManager::setLimits(int motorID, int limitCW, int limitCCW) {
	writeMotor(motorID, CW_ANGLE_LIMIT, limitCW);
	writeMotor(motorID, CCW_ANGLE_LIMIT, limitCCW);

}

void USB2AXmotorManager::connect() {
	if(dxl_initialize(0, 1))
		std::cout<<"Succeed to open USB2Dynamixel!"<<std::endl;
	else {
		std::cout<<"Failed to open USB2Dynamixel!, Moving to second port."<<std::endl;
		if (dxl_initialize(1, 1))
			std::cout<<"Succeed to open USB2Dynamixel!"<<std::endl;
		else
			std::cout<<"Failed to open USB2Dynamixel!"<<std::endl;
	}
}

void USB2AXmotorManager::disconnect() {
	dxl_terminate();
}

//Constructor
USB2AXmotorManager::USB2AXmotorManager() {
	connect();
	resetElapsedTime();
}

//Destructor
USB2AXmotorManager::~USB2AXmotorManager() {
	disconnect();
}
