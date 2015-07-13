

#ifndef USB2AXMOTORMANAGER_H_
#define USB2AXMOTORMANAGER_H_

//#include <opencv/highgui.h>
//#include <opencv/cv.h>
#include <ctime>
//#include <cstdio>
//#include <unistd.h>

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

//Motor
#define DEGREES_PER_POSITION	0.088
//#define RADIANS_PER_DEGREE		0.01745
#define NUM_MOTOR_POSITIONS 	4096

class USB2AXmotorManager {
	private:
		float minTime = 0.2; //Min time between dynamixel communication bursts (1 = 1s, 0.5 = 500ms)
		std::clock_t lastRequest;

		//std::vector<cv::Vec3i> motorLimits;
		//int errorCount = 0;
		//int errorLimit = 10;
		//std::set<int> motorIDs;

	public:
		USB2AXmotorManager(void);
		virtual ~USB2AXmotorManager(void);

		void initMoters(void);
		void connect(void);
		void disconnect(void);

		//void resetMotorMatrix(void);
		//void markSeenMotorMatrix(cv::Point);
		//cv::Point BFSmotorMatrix(cv::Point);

		bool checkMoving(int);
		void waitForStop(void);

		void printCommResult(void);

		//void incrementMotor(int, int);

		void addMotor(int);
		void setSpeed(int, int);
		void setAcceleration(int, int);
		void setLimits(int, int, int);

		//void markError(void);

		void setMotorPosition(int, int);
		int getMotorPosition(int);
		int getMotorPositionAccurate(int);

	private:
		void printErrorCode(void);
		void printCommStatus(int);
		bool checkElapsedTime(void);
		void resetElapsedTime(void);
		int readMotor(int, int);
		void writeMotor(int, int, int);
};

#endif /* HEADMODULE_H_ */
