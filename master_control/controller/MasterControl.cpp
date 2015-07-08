#include <stdio.h>
#include <unistd.h>
#include <VisionController.h>
#include <MasterControl.h>
#include <time.h>

extern "C"{
  #include <tx_serial.h>
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

 /* Boolean used for search function */
// bool mul8_knows_position = false;
// struct RoboCupGameControlData myData;

// int port;
// MasterControl mc;
int  number;

int main(){
  // int i;
  // int RxData;
  MasterControl mc;
  // MasterControl mc = new MasterControl();

  while (1) {
    // printf("%f\n", vc->getBallAngle());
  
    printf("Type in a number \n");
    
    if(scanf("%d", &number)){
      printf("Wrote: %d\n", number);

      if(number == 1){
        if (mc.executeMotion(GOFORWARD))
          fputs("success!\n", stderr);   
        else
          fputs("failure :(\n", stderr);

      }

      if(number == 2){
        if (mc.executeMotion(GOBACKWARD))
          fputs("success!\n", stderr);
        else
          fputs("failure :(\n", stderr);
      }
      
      if(number == 3){
        if (mc.executeMotion(STOP))
          fputs("success!\n", stderr);
        else
          fputs("failure :(\n", stderr);
      }

    }

    else {
      printf("That ain't a number ya donkus");
      break;    
    }


  }

  return 0;
}

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
  vc = new VisionController();
}

MasterControl::~MasterControl(){
  
}