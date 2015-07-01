#include <stdio.h>
#include <unistd.h>

extern "C"{
  #include <tx_serial.h>
}


int main(){

  int port = open_port();
  int i;
  int RxData;
  int TX_command;

	while (1) {

    printf("Type in a number \n");
    
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
    }

    // if (scanf("%d", &TX_command)){}

    // if (tx_data(port,TX_command)){}

    // for(i=0; i<TIMEOUT_TIME; i++){
    //   // Verify data recieved
    //   if(rx_check(port) == 1){
    //     // Get data verified
    //     RxData = rx_data();
    //     printf( "Recieved: %d\n", RxData );
    //     break;
    //   }
    //   printf("Loopin\n");

    //   sleep(1);
    // }

    // if(i == TIMEOUT_TIME) printf( "Timeout: Failed to recieve\n" );

    
  }

  return 0;
}