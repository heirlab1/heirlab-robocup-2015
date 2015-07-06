#include <stdio.h>
#include <unistd.h>
#include <VisionController.h>
#include <master_control.h>

extern "C"{
  #include <tx_serial.h>
}

int port;

int main(){

  port = open_port();
  // int i;
  // int RxData;
  int TX_command;
  VisionController *vc;
  vc = new VisionController();

	while (1) {
    printf("%f\n", vc->getBallAngle());


    /*printf("Type in a number \n");
    
    if(scanf("%d", &TX_command)){
      printf("Wrote: %d\n", TX_command);

      if(TX_command == 1){
        if (tx_data(port, GOFORWARD))
          fputs("success!\n", stderr);   
        else
          fputs("failure :(\n", stderr);

      }

      if(TX_command == 2){
        if (tx_data(port, GOBACKWARD))
          fputs("success!\n", stderr);
        else
          fputs("failure :(\n", stderr);
      }
    }

    else {
      printf("That ain't a number ya donkus");
      break;    
    }*/
    
  }

  return 0;
}

int execute_motion(int command){
  if (tx_data(port, command))
    fputs("success!\n", stderr);   
  else
    fputs("failure :(\n", stderr);
}