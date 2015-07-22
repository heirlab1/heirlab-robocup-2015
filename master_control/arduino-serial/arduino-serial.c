#include <stdio.h>    // Standard input/output definitions
#include <stdlib.h>
#include <string.h>   // String function definitions
#include <unistd.h>   // for usleep()
#include <getopt.h>

#include "arduino-serial-lib.h"
#include "arduino-serial.h"


void error(char const * msg)
{
    fprintf(stderr, "%s\n",msg);
    exit(EXIT_FAILURE);
}

// returns whether the start button has been pressed
int getStartButtonPressed(){
    const int buf_max = 256;

    int fd = -1;
    char const * serialport = "/dev/ttyACM0";
    int baudrate = 9600;  // default
    char quiet=0;
    char eolchar = '\n';
    int timeout = 5000;
    char buf[buf_max];

    // close port if already open
    if( fd!=-1 ) {
        serialport_close(fd);
        if(!quiet) printf("closed port %s\n",serialport);
    }

    // report on success or failure of opening port
    fd = serialport_init(serialport, baudrate);
    if( fd==-1 ) error("couldn't open arduino port");
    if(!quiet) printf("opened arduino port at: %s\n",serialport);
    serialport_flush(fd);
    if( fd == -1 ) error("serial port not opened");

    // used for empty test
    buf[0] = 0;
    // allocate memory
    memset(buf,0,buf_max);
    // read from the port
    serialport_read_until(fd, buf, eolchar, buf_max, timeout);
    // if pressed, return true
    if(buf[0] != 0){
        printf("Start button pressed\n");
        return 1;
    }
    return 0;
}
