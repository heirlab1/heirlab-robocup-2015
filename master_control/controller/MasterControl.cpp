#include <stdio.h>
#include <unistd.h>
#include <VisionController.h>
#include <Master_Control.h>

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

int port;
VisionController *vc;
MasterControl *mc;

/* Boolean used for search function */
bool mul8_knows_position = false;

struct RoboCupGameControlData myData;


int main(){

  port = open_port();
  // int i;
  // int RxData;
  vc = new Vision();
  mc = new MasterControl();

  while (1) {
    printf("%f\n", vc->getBallAngle());
  }

  return 0;
}

int execute_motion(int command){
  if (tx_data(port, command))
    fputs("success!\n", stderr);   
  else
    fputs("failure :(\n", stderr);
}

double MasterControl::getUnixTime(){

}

// action methods
void MasterControl::search(){

}

void MasterControl::walkTowardsBall(){
  execute_motion(GOFORWARD);
}

void MasterControl::getBehindBall(){

}

void MasterControl::alignToKick(){

}


// state methods

void MasterControl::init(){

}

void MasterControl::read(){

}

void MasterControl::play(){

}

void MasterControl::finish(){

}

void MasterControl::penalty(){

}


MasterControl::MasterControl(){

}

MasterControl::~MasterControl(){
  
}