#include <stdio.h>
#include <unistd.h>
#include <VisionController.h>
#include <MasterControl.h>
#include <time.h>
#include <pthread.h>

#include <Vision.h>
// #include <opencv/cv.h>
// #include <opencv/highgui.h>
// #include <unistd.h>

extern "C"{
  #include <tx_serial.h>
  #include <arduino-serial.h>
}


int MUL8_action = 0;
int MUL8_state = -1;

int currentThreshold = 85;
int firstThreshold = 80;
int secondThreshold = 70;

bool MUL8_initialized = false;
bool MUL8_ready = false;
bool MUL8_set = false;
bool MUL8_play_start = false;
bool MUL8_finished = false;
bool second_half = false;

int  number;




int main(){
  Vision vision;
  MasterControl mc;

  pthread_t sight, motion;
  pthread_create(&sight, NULL, sightLoop, NULL);
  //std::cout<<"Here"<<std::endl;
  pthread_create(&motion, NULL, motionLoop, NULL);
  pthread_join(sight, NULL);

  // find start button library in ../arduino-serial/ 
  while(!getStartButtonPressed())
  
  usleep(2000000);

  while (1) {
    if (getStartButtonPressed()) break;    
  }


  if (mc.executeMotion(STOP))
    fputs("stopped motion\n", stderr);
  else
    fputs("failed to stop motion(\n", stderr);
  return 0;



}


void* sightLoop(void* arg) {
  //vision.tracking.head.setPanAngle(-47);
  //vision.tracking.head.setTiltAngle(0);

  while(1) {
    //vision.ballObject.detect(vision.camera.getCameraImage());
    //std::cout<<"Pan: "<<vision.tracking.head.motorManager.getMotorPosition(24)<<std::endl;
    //std::cout<<"fdsf"<<std::endl;
    //Vision vision2;
    vision.ballObject.detect(vision.camera.getCameraImage());
    //cv::Mat imageHSV = vision.camera.getHSVImage();
    //vision.ballObject.detect(imageHSV);
    //goalObject.detect(imageHSV);
  }
  pthread_exit(NULL);
}

void* motionLoop(void* arg) {
  //vision.tracking.head.setPanAngle(-7);
  //vision.tracking.head.setTiltAngle(-19);
  //ballScreenParameters ball;

  while(1) {
    //vision.tracking.centerBallExperimental();
    //usleep(1000*100);
    //vision.tracking.searchBall();
    ballScreenParameters tempBall = vision.ballObject.getScreenParameters();
    vision.tracking.updatePhysicalParameters(tempBall);
    vision.tracking.centerBallExperimental(tempBall);
    //vision.tracking.head.setTiltAngle(0);
    //std::cout<<vision.tracking.head.getTiltAngle()<<std::endl;
    std::cout<<vision.ballObject.getPhysicalParameters().distance<<std::endl;
    /*if(tempBall.onScreen) {
      std::cout<<"Centering"<<std::endl;
      vision.tracking.head.motorManager.setSpeed(23, 70);
      vision.tracking.head.motorManager.setSpeed(24, 70);
      vision.tracking.centerBallExperimental(tempBall);
    }
    else {
      vision.tracking.head.motorManager.setSpeed(23, 20);
      vision.tracking.head.motorManager.setSpeed(24, 20);
      std::cout<<"Searching"<<std::endl;
      vision.tracking.searchBall();
    }*/
  }
  pthread_exit(NULL);
}

// find command defenitions in ../include/tx_serial.h
int MasterControl::executeMotion(int command){
  if (tx_data(port, command)) {
    fputs("success!\n", stderr);  
    return 1; 
  } else {
    fputs("failure :(\n", stderr);
  }
  return 0;
}

// double getUnixTime(){
//   struct timespec tv;

//   if (clock_gettime(CLOCK_REALTIME, &tv) != 0) {
//     return 0;
//   }

//   return (((double) tv.tv_sec) + (tv.tv_nsec / 1000000000.0));
// }

// action methods
void MasterControl::searchForBall(){
  // vc->setTask(TASK_LOOK_FOR_BALL);
}

void MasterControl::searchForGoal(){
  // vc->setTask(TASK_LOOK_FOR_GOAL);
}

void MasterControl::walkTowardsBall(){
  int x = executeMotion(GOFORWARD);
  printf("%d\n", x);
}

void MasterControl::getBehindBall(){

}

void MasterControl::alignToKick(){

}

// state methods
void MasterControl::init(){

}

void MasterControl::ready(){

}

void MasterControl::play(){

}

void MasterControl::finish(){

}

void MasterControl::penalty(){

}

MasterControl::MasterControl(){
  port = open_port();
  // vc = new VisionController();
}

MasterControl::~MasterControl(){
  
}