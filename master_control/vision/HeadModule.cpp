/*
 * HeadModule.cpp
 *
 *  Created on: Jun 12, 2015
 *      Author: ryan
 */

#include "HeadModule.h"
#include <dynamixel.h>
#include <stdio.h>
#include <set>
#include <queue>
#include <cmath>

void HeadModule::scan() {
	int counterMax = 10;
	int counter = 0;

	cv::Point moveToPos = cv::Point(0,0);
	cv::Point currentPos = cv::Point(0,0);
	while (1) {
		float areaSearched = 0;
		for(int i=0; i<(rightLimit-leftLimit); i++) { //Loop motorMatrix rows
				for(int j=0; j<(upperLimit-lowerLimit); j++) { //Loop motorMatrix columns
					if(motorMatrix[i][j]>=.5)
						areaSearched+=1;
			}
		}
		std::cout<<"%DNE: "<<areaSearched/((rightLimit-leftLimit)*(upperLimit-lowerLimit))<<std::endl;

		//std::cout<<"Try: "<<moveToPos<<std::endl;
		//std::cout<<"Is: "<<motorsReadPosition()<<std::endl;
		//usleep(50*1000);
		motorsWaitForStop();

		//usleep(1000*100);
		currentPos = moveToPos;
		markSeenMotorMatrix(currentPos);
		//std::cout<<"MotorMatrix "<<motorMatrix[upperLimit][rightLimit]<<std::endl;
		moveToPos = BFSmotorMatrix(currentPos);
		if(moveToPos.x == -1 && moveToPos.y == -1) {
			break;
		}
		else
			motorsMoveTo(moveToPos);
	}
	std::cout<<"Finished scanning"<<std::endl;
}

//Reads position for both motors and returns point
cv::Point HeadModule::motorsReadPosition() {
	int panMotor = motorRead(PAN_MOTOR_ID, PRESENT_POSITION);
	int tiltMotor = motorRead(TILT_MOTOR_ID, PRESENT_POSITION);
	return cv::Point(panMotor, tiltMotor);
	//return motorsPosition;
}

void HeadModule::motorsWaitForStop() {
	int maxCounter = 5;
	int count = 0;
	while(count<maxCounter) {
		usleep(1000);
		if(!motorsCheckMoving())
			count++;
		else
			count=0;
	}
}

//Check if motors are moving
bool HeadModule::motorsCheckMoving() {
	if(motorRead(PAN_MOTOR_ID, IS_MOVING) || motorRead(TILT_MOTOR_ID, IS_MOVING))
		return true;
	else
		return false;
}

//Moves selected motor to position
void HeadModule::motorMoveTo(int id, int pos) {
	if(id == PAN_MOTOR_ID) {
		//if(checkWithinLimits(cv::Point(motorsPosition.x, pos))) {
			motorWrite(id, GOAL_POSITION, pos);
			motorsPosition = cv::Point(motorsPosition.x, pos);
		//}
		//else
			//std::cout<<"Cannot move PAN motor there!"<<std::endl;
	}
	else if (id == TILT_MOTOR_ID) {
		//if(checkWithinLimits(cv::Point(pos, motorsPosition.y))) {
			motorWrite(id, GOAL_POSITION, pos);
			motorsPosition = cv::Point(pos, motorsPosition.y);
		//}
		//else
			//std::cout<<"Cannot move TILT motor there!"<<std::endl;
	}
}

//Moves both motors to desired position
void HeadModule::motorsMoveTo(cv::Point point) {
	if (checkWithinLimits(point)) {
		motorWrite(PAN_MOTOR_ID, GOAL_POSITION, point.x+rightLimit);
		motorWrite(TILT_MOTOR_ID, GOAL_POSITION, point.y+upperLimit);
		motorsPosition = point;
	}
	else
		std::cout<<"Cannot move motors there!"<<std::endl;
	/*}
	else
		std::cout<<"Error: motors are moving, cannot change course"<<std::endl;*/
}

//Reads desired command from selected motor
//Is private function for error handling
int HeadModule::motorRead(int id, int command) {
	while(!checkElapsedTime());
	int returnVal = dxl_read_word(id, command);
	resetElapsedTime();
	//printCommResult();
	//std::cout<<command<<std::endl;
	return returnVal;
}

//Writes desired command to selected motor
//Is private function for error handling
void HeadModule::motorWrite(int id , int command, int value) {
	while(!checkElapsedTime());
	dxl_write_word(id, command, value);
	resetElapsedTime();
	//printCommResult();
	//std::cout<<command<<std::endl;
}

void HeadModule::motorIncrement(int id, int amount) {
	if(id==PAN_MOTOR_ID) {
		if(checkWithinLimits(cv::Point(motorsPosition.x+amount, motorsPosition.y))) {
			motorsPosition=cv::Point(motorsPosition.x+amount, motorsPosition.y);
			motorWrite(PAN_MOTOR_ID, GOAL_POSITION, motorsPosition.x);
		}
		else
			std::cout<<"Cannot increment PAN motor there!"<<std::endl;

	}
	else if(id ==TILT_MOTOR_ID) {
		if(checkWithinLimits(cv::Point(motorsPosition.x, motorsPosition.y+amount))) {
		motorsPosition=cv::Point(motorsPosition.x, motorsPosition.y+amount);
		motorWrite(TILT_MOTOR_ID, GOAL_POSITION, motorsPosition.y);
		}
		else
			std::cout<<"Cannot increment TILT motor there!"<<std::endl;
	}
}

//Returns true if given poiunt is within the movement limits
bool HeadModule::checkWithinLimits(cv::Point point) {
	if(point.x > rightLimit-leftLimit)
		return false;
	else if(point.x < 0)
		return false;
	else if(point.y > upperLimit-lowerLimit)
		return false;
	else if(point.y < 0)
		return false;
	else
		return true;
}

//Checks to see if time since last request has passed
bool HeadModule::checkElapsedTime() {
	std::clock_t currentTime = clock();
	if (float(currentTime-lastRequest)/CLOCKS_PER_SEC > 0.09)
		return true;
	else
		return false;
}

//Resets time since last request to current time
void HeadModule::resetElapsedTime() {
	lastRequest = clock();
}

//Finds Errors in transmission and prints
void HeadModule::printCommResult() {
	int commStatus = dxl_get_result();
	if (commStatus == COMM_RXSUCCESS)
		printErrorCode();
	else
		printCommStatus(commStatus);
}

//Prints error code
void HeadModule::printErrorCode() {
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
void HeadModule::printCommStatus(int commStatus) {
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

//Resets motor matrix to 0s
void HeadModule::resetMotorMatrix() {
	for(int i=0; i<(rightLimit-leftLimit); i++) { //Loop motorMatrix rows
		for(int j=0; j<(upperLimit-lowerLimit); j++) { //Loop motorMatrix columns
			motorMatrix[i][j]=0; //[X][Y]
		}
	}
}

//Increases the seen number in mototMatrix to mark how well seen this point has been during a search
void HeadModule::markSeenMotorMatrix(cv::Point centerPoint) {
	int upperLimitTemp = centerPoint.y+cameraTiltLimit;
	int lowerLimitTemp = centerPoint.y-cameraTiltLimit;
	int rightLimitTemp = centerPoint.x+cameraPanLimit;
	int leftLimitTemp = centerPoint.x-cameraPanLimit;

	if(centerPoint.x+cameraPanLimit > rightLimit-leftLimit)
		rightLimitTemp = rightLimit-leftLimit;
	if(centerPoint.x-cameraPanLimit < 0)
		leftLimitTemp = 0;
	if(centerPoint.y+cameraTiltLimit > upperLimit-lowerLimit)
		upperLimitTemp = upperLimit-lowerLimit;
	if(centerPoint.x-cameraTiltLimit < 0)
		lowerLimitTemp = 0;

	for(int i=lowerLimitTemp; i<upperLimitTemp; i++) { //Loop motorMatrix rows
		for(int j=leftLimitTemp; j<rightLimitTemp; j++) { //Loop motorMatrix columns
			float distance = std::sqrt((centerPoint.x-j)*(centerPoint.x-j)+(centerPoint.y-i)*(centerPoint.y-i));

			if(distance<.10*cameraTiltLimit) //If in 10% of the center
				motorMatrix[i][j]=motorMatrix[i][j]+threshold;
			else if(distance<.25*cameraTiltLimit) //If 25% from center
				motorMatrix[i][j]=motorMatrix[i][j]+1*threshold;
			else if(distance<.50*cameraTiltLimit) //If 50% from center
				motorMatrix[i][j]=motorMatrix[i][j]+1*threshold;
			else if(distance<.75*cameraTiltLimit) //If 75% from center
				motorMatrix[i][j]=motorMatrix[i][j]+1*threshold;
			else if(distance<.90*cameraTiltLimit) //If 90% from center
				motorMatrix[i][j]=motorMatrix[i][j]+1*threshold;
		}
	}
}

void HeadModule::stopHead() {
	int tiltPos = motorRead(TILT_MOTOR_ID, PRESENT_POSITION);
    int panPos = motorRead(PAN_MOTOR_ID, PRESENT_POSITION);
    motorWrite(TILT_MOTOR_ID, GOAL_POSITION, tiltPos);
    motorWrite(PAN_MOTOR_ID, GOAL_POSITION, panPos);
}

//Scans for nearest point that unsearched enough and returns it
cv::Point HeadModule::BFSmotorMatrix(cv::Point startPoint) {

	std::queue<cv::Point> searchQueue;
	std::queue<cv::Point> finalQueue;

	//int searchedPoints[upperLimit-lowerLimit][rightLimit-leftLimit];
	bool searchedPoints[upperLimit-lowerLimit][rightLimit-leftLimit]; //[X][Y]
	for(int i=0; i<(rightLimit-leftLimit); i++) { //Loop motorMatrix rows
		for(int j=0; j<(upperLimit-lowerLimit); j++) { //Loop motorMatrix columns
			searchedPoints[i][j]=false; //[X][Y]
		}
	}

	cv::Point lowestValuePoint, tempPoint; //lowestValuePoint is placeholder for return value
	std::set<float>::iterator iterator;

	searchQueue.push(cv::Point(startPoint.x, startPoint.y)); //Push current point

	searchedPoints[tempPoint.x][tempPoint.y]=true;
	//While nothing is found and the entire image hasn't been searched
	while(finalQueue.empty() && searchQueue.size() != 0) {
		//For this task of queue entries, process
		int searchQueueSize = searchQueue.size();

		//std::cout<<"QueueSize: "<<searchQueueSize<<std::endl;
		//std::cout<<"FinalSize: "<<finalQueue.size()<<std::endl;


		for(int i=0; i<searchQueueSize; i++) {
			tempPoint = searchQueue.front(); //Take element from queue
			searchQueue.pop(); //Remove element from queue

			//std::cout<<tempPoint<<std::endl;

			// If within a given threshold, add to final queue
			if(motorMatrix[tempPoint.y][tempPoint.x]<threshold)
				finalQueue.push(tempPoint);
			else {
				//Searches adjacent points (up, down, right, left) in graph to see if they have been explored. If not, add to search queue
				//Outer if statements make sure we are not searching outside our head limitation angles
				//Inner if statements check to see if the elements we want to add have not previously been searched
				if(tempPoint.x+motorMatrixAccuracy < rightLimit-leftLimit) {
					if(!searchedPoints[tempPoint.x+motorMatrixAccuracy][tempPoint.y]) {
						searchQueue.push(cv::Point(tempPoint.x+motorMatrixAccuracy, tempPoint.y));
						searchedPoints[tempPoint.x+motorMatrixAccuracy][tempPoint.y]=true;
					}
				}
				if(tempPoint.x-motorMatrixAccuracy > 0) {
					if(!searchedPoints[tempPoint.x-motorMatrixAccuracy][tempPoint.y]) {
						searchQueue.push(cv::Point(tempPoint.x-motorMatrixAccuracy, tempPoint.y));
						searchedPoints[tempPoint.x-motorMatrixAccuracy][tempPoint.y]=true;
					}
				}
				if(tempPoint.y+motorMatrixAccuracy < upperLimit-lowerLimit) {
					if(!searchedPoints[tempPoint.x][tempPoint.y+motorMatrixAccuracy]) {
						searchQueue.push(cv::Point(tempPoint.x, tempPoint.y+motorMatrixAccuracy));
						searchedPoints[tempPoint.x][tempPoint.y+motorMatrixAccuracy]=true;
					}
				}
				if(tempPoint.y-motorMatrixAccuracy > 0) {
					if(!searchedPoints[tempPoint.x][tempPoint.y-motorMatrixAccuracy]) {
						searchQueue.push(cv::Point(tempPoint.x, tempPoint.y-motorMatrixAccuracy));
						searchedPoints[tempPoint.x][tempPoint.y-motorMatrixAccuracy]=true;
					}
				}
			}//End else
		}//End For
	} //End while

	if(!finalQueue.empty()) {
		lowestValuePoint = finalQueue.front(); //place
		finalQueue.pop();

		//Find element with lowest number for returning
		for(int i=finalQueue.size(); 0<i; i--) {
			tempPoint = finalQueue.front();
			if(motorMatrix[tempPoint.x][tempPoint.y] < motorMatrix[lowestValuePoint.x][lowestValuePoint.y])
				lowestValuePoint = finalQueue.front();
			finalQueue.pop();
		}
		return lowestValuePoint;
	}
	else
		return cv::Point(-1,-1); //Method failed to find anything and needs to be reset
}


/*cv::Point HeadModule::BFSmotorMatrix(cv::Point startPoint) {

	std::queue<cv::Point> searchQueue;
	std::queue<cv::Point> finalQueue;
	std::set<cv::Point, comparePoints> markedPoints;
	cv::Point lowestValuePoint, tempPoint; //lowestValuePoint is placeholder for return value

	std::set<cv::Point>::iterator iterator;
	searchQueue.push(cv::Point(startPoint.x, startPoint.y));

	//std::cout<<rightLimit-leftLimit<<" "<<upperLimit-lowerLimit<<std::endl;
	//While nothing is found and the entire image hasn't been searched
	while(finalQueue.empty() && searchQueue.size() != 0) {
		//For this task of queue entries, process
		int searchQueueSize = searchQueue.size();


		std::cout<<"QueueSize: "<<searchQueueSize<<std::endl;
		std::cout<<"SeenSize: "<<markedPoints.size()<<std::endl;
		//std::cout<<"FinalSize: "<<finalQueue.size()<<std::endl;


		for(int i=0; i<searchQueueSize; i++) {
			tempPoint = searchQueue.front(); //Take element from queue
			searchQueue.pop(); //Remove element from queue

			//std::cout<<tempPoint.x<<std::endl;
			markedPoints.insert(cv::Point(tempPoint.x, tempPoint.y)); //Mark current point as seen
			// If within a given threshold, add to final queue
			if(motorMatrix[tempPoint.y][tempPoint.x]<threshold)
				finalQueue.push(tempPoint);
			else {
				//Searches adjacent points (up, down, right, left) in graph to see if they have been explored. If not, add to search queue
				//Outer if statements make sure we are not searching outside our head limitation angles
				//Inner if statements check to see if the elements we want to add have not previously been searched
				if(tempPoint.x+motorMatrixAccuracy < rightLimit-leftLimit) {
					iterator = markedPoints.find(cv::Point(tempPoint.x+motorMatrixAccuracy, tempPoint.y)); //Search adjacent right
					if(iterator != markedPoints.end())
					//if(!markedPoints.count(cv::Point(tempPoint.x+motorMatrixAccuracy, tempPoint.y)))
						searchQueue.push(cv::Point(tempPoint.x+motorMatrixAccuracy, tempPoint.y));
				}
				if(tempPoint.x-motorMatrixAccuracy > 0) {
					iterator = markedPoints.find(cv::Point(tempPoint.x-motorMatrixAccuracy, tempPoint.y)); //Search adjacent left
					if(iterator != markedPoints.end())
					//if(!markedPoints.count(cv::Point(tempPoint.x-motorMatrixAccuracy, tempPoint.y)))
						searchQueue.push(cv::Point(tempPoint.x-motorMatrixAccuracy, tempPoint.y));
				}
				if(tempPoint.y+motorMatrixAccuracy < upperLimit-lowerLimit) {
					iterator = markedPoints.find(cv::Point(tempPoint.x, tempPoint.y+motorMatrixAccuracy)); //Search adjacent up
					if(iterator != markedPoints.end())
					//if(!markedPoints.count(cv::Point(tempPoint.x, tempPoint.y+motorMatrixAccuracy)))
						searchQueue.push(cv::Point(tempPoint.x, tempPoint.y+motorMatrixAccuracy));
				}
				if(tempPoint.y-motorMatrixAccuracy > 0) {
					iterator = markedPoints.find(cv::Point(tempPoint.x, tempPoint.y-motorMatrixAccuracy)); //Search adjacent down
					if(iterator != markedPoints.end())
					//if(!markedPoints.count(cv::Point(tempPoint.x, tempPoint.y+motorMatrixAccuracy)))
						searchQueue.push(cv::Point(tempPoint.x, tempPoint.y-motorMatrixAccuracy));
				}
			}//End else
		}//End For
	} //End while

	if(!finalQueue.empty()) {
		lowestValuePoint = finalQueue.front(); //place
		finalQueue.pop();

		//Find element with lowest number for returning
		for(int i=finalQueue.size(); 0<i; i--) {
			tempPoint = finalQueue.front();
			if(motorMatrix[tempPoint.y][tempPoint.x] < motorMatrix[lowestValuePoint.y][lowestValuePoint.x])
				lowestValuePoint = finalQueue.front();
			finalQueue.pop();
		}
		return lowestValuePoint;
	}
	else
		return cv::Point(-1000,-1000); //Method failed to find anything and needs to be reset
}*/

//Initializes motor positions
void HeadModule::initMoters() {
	///////// Open USB2Dynamixel ////////////
	if(dxl_initialize(0, 1)==1)
		std::cout<<"Succeed to open USB2Dynamixel!"<<std::endl;
	else if (dxl_initialize(1, 1)==1)
		std::cout<<"Failed to open USB2Dynamixel!, Moving to second port."<<std::endl;
	else
		std::cout<<"Failed to open USB2Dynamixel!"<<std::endl;

	//dxl_write_word(24, LED_ENABLE, 1);

	offsetPosHorizontal = 0;
	offsetPosVertical = 0;

	resetElapsedTime();
	std::cout<<"Start"<<std::endl;

	//motorWrite(PAN_MOTOR_ID, CW_ANGLE_LIMIT, 0);
	//motorWrite(PAN_MOTOR_ID, CCW_ANGLE_LIMIT, 4000);
	//motorWrite(TILT_MOTOR_ID, CW_ANGLE_LIMIT, 0);
	//motorWrite(TILT_MOTOR_ID, CCW_ANGLE_LIMIT, 4000);

	//std::cout<<"Delay Pan: "<<motorRead(PAN_MOTOR_ID, 4)<<std::endl;
	//std::cout<<"Delay Tilt: "<<motorRead(TILT_MOTOR_ID, 4)<<std::endl;
	motorWrite(PAN_MOTOR_ID, CW_ANGLE_LIMIT, 0);
	motorWrite(PAN_MOTOR_ID, CCW_ANGLE_LIMIT, 4095);
	motorWrite(TILT_MOTOR_ID, CW_ANGLE_LIMIT, 0);
	motorWrite(TILT_MOTOR_ID, CCW_ANGLE_LIMIT, 4095);

	motorWrite(PAN_MOTOR_ID, MOVING_SPEED, 40);
	motorWrite(TILT_MOTOR_ID, MOVING_SPEED, 40);

	//motorWrite(PAN_MOTOR_ID, TORQUE_ENABLE, 1);
	//motorWrite(TILT_MOTOR_ID, TORQUE_ENABLE, 1);

	//motorWrite(PAN_MOTOR_ID, GOAL_POSITION, rightLimit);
	//motorWrite(TILT_MOTOR_ID, GOAL_POSITION, upperLimit);
	motorsPosition = cv::Point(rightLimit, upperLimit);

	std::cout<<"End"<<std::endl;

}

//Constructor
HeadModule::HeadModule() {
	this->initMoters();
	//this->scan();
	usleep(1000*3000);
	motorWrite(PAN_MOTOR_ID, TORQUE_ENABLE, 0);
	//motorWrite(PAN_MOTOR_ID, GOAL_POSITION, 4095/2);
	//motorWrite(TILT_MOTOR_ID, GOAL_POSITION, 4095/2);
	std::cout<<"End"<<std::endl;
	usleep(1000*5000);
	//motorWrite(PAN_MOTOR_ID, GOAL_POSITION, (int)(4095/2));

	while(1){
		usleep(1000*2000);
		std::cout<<"Position: "<<motorRead(PAN_MOTOR_ID, PRESENT_POSITION)<<std::endl;
	}
}

//Destructor
HeadModule::~HeadModule() {
	dxl_terminate();
}

	/*int getMotorPositionReadWord(int id) {
	        return dxl_read_word(id, PRESENT_POSITION);
	}

	int getMotorPositionReadWord(int id) {
	        return dxl_read_word(id, PRESENT_POSITION);
	}

	void calibrateMotor(int motor, int adjust) {
	        for(int i = 0; i<currMo.length; i++){
	                std::cout<< "Changing Motor "<< motor << " from "<<currMo.motorPositions[i][motor-1];
	                currMo.motorPositions[i][motor-1]+= adjust;
	                std::cout<< "to"  <<currMo.motorPositions[i][motor-1]<<std::endl;
	        }
	        recalculateCurrentMotionSpeeds();

	}

	void getTorqueReadings() {
		for(int j= 0; j<currMo.num_motors; j++) {
			currMo.torqueReadings[currMo.currentIndex][j]= dxl_read_word(PRESENT_LOAD, j+1);
		}

	}
	void printTorqueReadings() {
	        std::ofstream outFile;
	        std::string extension = currMo.friendlyName +"_torque.data";

	        std::string wholePath = DATA_PREPATH + extension; //show the path of the motion file

	        outFile.open(wholePath.c_str()); //open this path

	        //Writing the current motion struct into this .mtn file

	        for(int i= 0; i<currMo.num_motors; i++){

	                for(int j= 0; j<currMo.length; j++){
	                        outFile<< currMo.torqueReadings[j][i]<<"\t";
	                }
	                outFile<< "\n";

	        }
	        outFile.close();
	}

	std::vector<int> getMotorPositionSyncRead(int numMotors) {
	        std::vector<int> positions;
	        positions.resize(numMotors);
	        int ids[numMotors];

	        for (int i = 0; i < numMotors; i++) {
	                ids[i] = i+1;
	                std::cout << ids[i] << " ";
	        }

	        std::cout << std::endl;


	        dxl_set_txpacket_id(0xFD);//ID of destination
	        dxl_set_txpacket_instruction(INST_SYNC_READ);// packet type
	        dxl_set_txpacket_parameter(0, PRESENT_POSITION);//which data to manipulate. speed, goal pos, ect
	        dxl_set_txpacket_parameter(1, BYTE);//how long the instruction will be.

	        for(int i=0; i<numMotors; i++) {
	                // Set the ID number
	                dxl_set_txpacket_parameter(2+i, ids[i]);
	        }

	        dxl_set_txpacket_length(numMotors+4);
	        dxl_txrx_packet();//sends the packet

	        int high;
	        int low;
	        std::cout << "Total packets: " << dxl_get_rxpacket_length() << std::endl;

	        for (int i = 0; i < numMotors; i++) {
	                low = dxl_get_rxpacket_parameter(2*i);
	                std::cout << "Low: " << low << "\t\t";
	                high = dxl_get_rxpacket_parameter(2*i+1);
	                std::cout << "High: " << high << "\t\t" << i << std::endl;
	                positions[i] = dxl_makeword(low, high);
	        }



	        return positions;
	}

	void setTorqueLimit(int id, int torque) { //set new torque limits
	      dxl_write_word(id, MAX_TORQUE, torque);
	}

	void disableMotor(int motor) {
	        dxl_write_byte(motor, TORQUE_ENABLE, 0);
	}

	void enableMotor(int motor) {
	        dxl_write_byte(); (motor, TORQUE_ENABLE, 1);
	        //      std::cout << "Present Position of motor " << motor << ": " << dxl_read_word(motor, PRESENT_POSITION) << std::endl;
	}


	void init() {

	    // Initialize USB2Dynamixel
	    dxl_initialize(0,1);

	    //Initialize motors



	    initialize();
	    // Set the current motion to standing
	    currentMotion = "Stand";


	    motors.resize(TOTAL_MOTORS);
	    for (int i = 0; i < TOTAL_MOTORS; i++) {
	            motors[i] = i + 1;
	    }
	    //std::cout<<"resized motors array"<<std::endl;
	    data.resize(TOTAL_MOTORS);
	    //std::cout<<"resized data for syncwrite array"<<std::endl;
	}

	void moveHead(int direction, int speed) {
		switch (direction) {
			case MUL8_HEAD_UP:
				// Write the moving speed to motor 24
				dxl_write_word(24, MOVING_SPEED, speed);
				// Set the goal position as the farthest back we can go
				dxl_write_word(24, GOAL_POSITION, M24_CCW);
				dxl_write_word(23, GOAL_POSITION, dxl_read_word(23, PRESENT_POSITION));
				break;
			case MUL8_HEAD_DOWN:
				// Write the moving speed to motor 24
				dxl_write_word(24, MOVING_SPEED, speed);
				// Set the goal position as the farthest forward we can go
				dxl_write_word(24, GOAL_POSITION, M24_CW);
				dxl_write_word(23, GOAL_POSITION, dxl_read_word(23, PRESENT_POSITION));
				break;
			case MUL8_HEAD_LEFT:
				// Write the moving speed to motor 23
				dxl_write_word(23, MOVING_SPEED, speed);
				// Write the goal position as the farthest left we can go
				dxl_write_word(23, GOAL_POSITION, M23_CCW);
				dxl_write_word(24, GOAL_POSITION, dxl_read_word(24, PRESENT_POSITION));
				break;
			case MUL8_HEAD_RIGHT:
				// Write the moving speed to motor 23
				dxl_write_word(23, MOVING_SPEED, speed);
				// Set the goal position as the farthest right we can go
				dxl_write_word(23, GOAL_POSITION, M23_CW);
				dxl_write_word(24, GOAL_POSITION, dxl_read_word(24, PRESENT_POSITION));
				break;
			case MUL8_HEAD_UP_LEFT:
				// Write the moving speed to motors 23 and 24
				dxl_write_word(23, MOVING_SPEED, speed/sqrt(2));
				dxl_write_word(24, MOVING_SPEED, speed/sqrt(2));
				// Set the goal position as the farthest left and up we can go
				dxl_write_word(23, GOAL_POSITION, M23_CCW);
				dxl_write_word(24, GOAL_POSITION, M24_CCW);
				break;
			case MUL8_HEAD_DOWN_LEFT:
				// Set the speed for both motors 23 and 24
				dxl_write_word(23, MOVING_SPEED, speed/sqrt(2));
				dxl_write_word(24, MOVING_SPEED, speed/sqrt(2));
				// Set the goal position as the farthest left and down we can go
				dxl_write_word(23, GOAL_POSITION, M23_CCW);
				dxl_write_word(24, GOAL_POSITION, M24_CW);
				break;
			case MUL8_HEAD_UP_RIGHT:
				// Write the moving speed to motors 23 and 24
				dxl_write_word(23, MOVING_SPEED, speed/sqrt(2));
				dxl_write_word(24, MOVING_SPEED, speed/sqrt(2));
				// Set the goal position as the farthest up and right we can go
				dxl_write_word(23, GOAL_POSITION, M23_CW);
				dxl_write_word(24, GOAL_POSITION, M24_CCW);
				break;
			case MUL8_HEAD_DOWN_RIGHT:
				// Write the moving speed to motors 23 and 24
				dxl_write_word(23, MOVING_SPEED, speed/sqrt(2));
				dxl_write_word(24, MOVING_SPEED, speed/sqrt(2));
				// Set the goal position as the farthest down and right we can go
				dxl_write_word(23, GOAL_POSITION, M23_CW);
				dxl_write_word(24, GOAL_POSITION, M24_CW);
				break;
			default:
				break;
}
	}

	// Print communication result
	void PrintCommStatus(int CommStatus) {
	        switch(CommStatus) {
	        	case COMM_TXFAIL:
	                printf("COMM_TXFAIL: Failed transmit instruction packet!\n");
	                break;
	        	case COMM_TXERROR:
	                printf("COMM_TXERROR: Incorrect instruction packet!\n");
	                break;
	        	case COMM_RXFAIL:
	                printf("COMM_RXFAIL: Failed get status packet from device!\n");
	                break;
	        	case COMM_RXWAITING:
	                printf("COMM_RXWAITING: Now recieving status packet!\n");
	                break;
	        	case COMM_RXTIMEOUT:
	                printf("COMM_RXTIMEOUT: There is no status packet!\n");
	                break;
	        	case COMM_RXCORRUPT:
	                printf("COMM_RXCORRUPT: Incorrect status packet!\n");
	                break;
	        	default:
	                printf("This is unknown error code!\n");
	                break;
	        }
	}

	void PrintErrorCode() {
	        if(dxl_get_rxpacket_error(ERRBIT_VOLTAGE) == 1)
	                printf("Input voltage error!\n");
	        if(dxl_get_rxpacket_error(ERRBIT_ANGLE) == 1)
	                printf("Angle limit error!\n");
	        if(dxl_get_rxpacket_error(ERRBIT_OVERHEAT) == 1)
	                printf("Overheat error!\n");
	        if(dxl_get_rxpacket_error(ERRBIT_RANGE) == 1)
	                printf("Out of range error!\n");
	        if(dxl_get_rxpacket_error(ERRBIT_CHECKSUM) == 1)
	                printf("Checksum error!\n");
	        if(dxl_get_rxpacket_error(ERRBIT_OVERLOAD) == 1)
	                printf("Overload error!\n");
	        if(dxl_get_rxpacket_error(ERRBIT_INSTRUCTION) == 1)
	                printf("Instruction code error!\n");
	}*/



/*
void MotorController::setCompliantLimb(int compliancy){
        currentTorque= compliancy;
}

void MotorController::setTorqueLimit(int id, int torque) {            //set new torque limits
      dxl_write_word(id, MAX_TORQUE, torque);
}

void MotorController::disableMotor(int motor) {
        dxl_write_byte(motor, TORQUE_ENABLE, 0);
}

void MotorController::enableMotor(int motor) {
        dxl_write_byte(motor, TORQUE_ENABLE, 1);
        //      std::cout << "Present Position of motor " << motor << ": " << dxl_read_word(motor, PRESENT_POSITION) << std::endl;
}




void MotorController::moveHead(int direction, int speed) {
        switch (direction) {
        case MUL8_HEAD_UP:
                // Write the moving speed to motor 24
                dxl_write_word(24, MOVING_SPEED, speed);
                // Set the goal position as the farthest back we can go
                dxl_write_word(24, GOAL_POSITION, M24_CCW);
                dxl_write_word(23, GOAL_POSITION, dxl_read_word(23, PRESENT_POSITION));
                break;
        case MUL8_HEAD_DOWN:
                // Write the moving speed to motor 24
                dxl_write_word(24, MOVING_SPEED, speed);
                // Set the goal position as the farthest forward we can go
                dxl_write_word(24, GOAL_POSITION, M24_CW);
                dxl_write_word(23, GOAL_POSITION, dxl_read_word(23, PRESENT_POSITION));
                break;
        case MUL8_HEAD_LEFT:
                // Write the moving speed to motor 23
                dxl_write_word(23, MOVING_SPEED, speed);
                // Write the goal position as the farthest left we can go
                dxl_write_word(23, GOAL_POSITION, M23_CCW);
                dxl_write_word(24, GOAL_POSITION, dxl_read_word(24, PRESENT_POSITION));
                break;
        case MUL8_HEAD_RIGHT:
                // Write the moving speed to motor 23
                dxl_write_word(23, MOVING_SPEED, speed);
                // Set the goal position as the farthest right we can go
                dxl_write_word(23, GOAL_POSITION, M23_CW);
                dxl_write_word(24, GOAL_POSITION, dxl_read_word(24, PRESENT_POSITION));
                break;
        case MUL8_HEAD_UP_LEFT:
                // Write the moving speed to motors 23 and 24
                dxl_write_word(23, MOVING_SPEED, speed/sqrt(2));
                dxl_write_word(24, MOVING_SPEED, speed/sqrt(2));
                // Set the goal position as the farthest left and up we can go
                dxl_write_word(23, GOAL_POSITION, M23_CCW);
                dxl_write_word(24, GOAL_POSITION, M24_CCW);
                break;
        case MUL8_HEAD_DOWN_LEFT:
                // Set the speed for both motors 23 and 24
                dxl_write_word(23, MOVING_SPEED, speed/sqrt(2));
                dxl_write_word(24, MOVING_SPEED, speed/sqrt(2));
                // Set the goal position as the farthest left and down we can go
                dxl_write_word(23, GOAL_POSITION, M23_CCW);
                dxl_write_word(24, GOAL_POSITION, M24_CW);
                break;

        case MUL8_HEAD_UP_RIGHT:
                // Write the moving speed to motors 23 and 24
                dxl_write_word(23, MOVING_SPEED, speed/sqrt(2));
                dxl_write_word(24, MOVING_SPEED, speed/sqrt(2));
                // Set the goal position as the farthest up and right we can go
                dxl_write_word(23, GOAL_POSITION, M23_CW);
                dxl_write_word(24, GOAL_POSITION, M24_CCW);
                break;
        case MUL8_HEAD_DOWN_RIGHT:
                // Write the moving speed to motors 23 and 24
                dxl_write_word(23, MOVING_SPEED, speed/sqrt(2));
                dxl_write_word(24, MOVING_SPEED, speed/sqrt(2));
                // Set the goal position as the farthest down and right we can go
                dxl_write_word(23, GOAL_POSITION, M23_CW);
                dxl_write_word(24, GOAL_POSITION, M24_CW);
                break;
        default:
                break;
        }
}

void MotorController::stopHead() {
        int motor23pos = dxl_read_word(23, PRESENT_POSITION);
        int motor24pos = dxl_read_word(24, PRESENT_POSITION);
        dxl_write_word(23, GOAL_POSITION, motor23pos);
        dxl_write_word(24, GOAL_POSITION, motor24pos);
}

bool MotorController::headLeftRightIsMoving() {
        bool result = false;
        int present23pos = dxl_read_word(23, PRESENT_POSITION);
        int goal23pos = dxl_read_word(23, GOAL_POSITION);
//      std::cout << "23: " << present23pos << " vs " << goal23pos << std::endl;
        if (present23pos < (goal23pos-25) || (present23pos > goal23pos+25)) {
//              std::cout << "Returning true" << std::endl;
                result = true;
        }
        return result;
}

bool MotorController::headUpDownIsMoving() {
        bool result = false;
        int present24pos = dxl_read_word(24, PRESENT_POSITION);
        int goal24pos = dxl_read_word(24, GOAL_POSITION);
//      std::cout << "24: " << present24pos << " vs " << goal24pos << std::endl;
        if (present24pos < (goal24pos-25) || present24pos > goal24pos+25) {
                result = true;
        }
        return result;
}
*/
