#include <stdio.h>
#include <tx_serial.h>
#include <master_control.h>

main(){
	while (true) {
    int  number;
    printf("Type in a number \n");
    scanf("%d", &number);
    printf("Wrote: %d\n", number);

    if(number == 1){
      if (tx_data(FORWARD))
        fputs("success!\n", stderr);   
      else
        fputs("failure :(\n", stderr);

    }

    if(number == 2){
      if (tx_data(BACKWARD))
        fputs("success!\n", stderr);
      else
        fputs("failure :(\n", stderr);
    }
  }
}