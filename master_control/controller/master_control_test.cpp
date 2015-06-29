#include <stdio.h>
#include <tx_serial.h>

int main(){

  int port = open_port();
  int i;
  int RxData;
  int Tx_Data;

	while (1) {

    int  number;
    printf("Type in a number \n");
    
    // if(scanf("%d", &number)){
    //   printf("Wrote: %d\n", number);

    //   if(number == 1){
    //     if (tx_data(port, FORWARD))
    //       fputs("success!\n", stderr);   
    //     else
    //       fputs("failure :(\n", stderr);

    //   }

    //   if(number == 2){
    //     if (tx_data(port, BACKWARD))
    //       fputs("success!\n", stderr);
    //     else
    //       fputs("failure :(\n", stderr);
    //   }
    // }

    // else {
    //   printf("That ain't a number ya donkus");
    //   break;    
    // }

    if (scanf("%d", &Tx_Data)){}

    if (tx_data(port,Tx_Data)){}

    for(i=0; i<TIMEOUT_TIME; i++){
      // Verify data recieved
      if(rx_check(port) == 1){
        // Get data verified
        RxData = rx_data();
        printf( "Recieved: %d\n", RxData );
        break;
      }
      printf("Loopin\n");

      sleep(1);
    }

    if(i == TIMEOUT_TIME) printf( "Timeout: Failed to recieve\n" );

    
  }

  return 0;
}