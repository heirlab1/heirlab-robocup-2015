#include <stdio.h>
#include <tx_serial.h>

int main(){

  int port = open_port();
  
	while (1) {
    int  number;
    printf("Type in a number \n");
    
    if(scanf("%d", &number)){
      printf("Wrote: %d\n", number);

      if(number == 1){
        if (tx_data(port, FORWARD))
          fputs("success!\n", stderr);   
        else
          fputs("failure :(\n", stderr);

      }

      if(number == 2){
        if (tx_data(port, BACKWARD))
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