#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <zigbee.h>

/* baudrate settings are defined in <asm/termbits.h>, which is
included by <termios.h> */
#define BAUDRATE B57600            
/* change this definition for the correct port */
#define MODEMDEVICE "/dev/ttyUSB0"
#define _POSIX_SOURCE 1 /* POSIX compliant source */

#define FALSE 0
#define TRUE 1
#define DEFAULT_DEVICEINDEX 0

// #define UP    "\xFF\x55\x01\xFE\x00\xFF"
// #define DOWN  "\xFF\x55\x02\xFD\x00\xFF"
// #define LEFT  "\xFF\x55\x04\xFB\x00\xFF"
// #define RIGHT "\xFF\x55\x08\xF7\x00\xFF"

// #define _6    "\xFF\x55\x00\xFF\x00\xFF"
// #define _5    "\xFF\x55\x00\xFF\x01\xFE"
// #define _4    "\xFF\x55\x80\x7F\x00\xFF"
// #define _3    "\xFF\x55\x40\xBF\x00\xFF"
// #define _2    "\xFF\x55\x20\xDF\x00\xFF"
// #define _1    "\xFF\x55\x10\xEF\x00\xFF"

// #define _1U   "\xFF\x55\x11\xEE\x00\xFF"
// #define _1D   "\xFF\x55\x12\xED\x00\xFF"
// #define _1L   "\xFF\x55\x14\xEB\x00\xFF"
// #define _1R   "\xFF\x55\x18\xEF\x00\xFF"

// #define _2U   "\xFF\x55\x21\xDE\x00\xFF"
// #define _2D   "\xFF\x55\x22\xDD\x00\xFF"
// #define _2L   "\xFF\x55\x24\xDB\x00\xFF"
// #define _2R   "\xFF\x55\x28\xD7\x00\xFF"

// #define _3U   "\xFF\x55\x41\xBE\x00\xFF"
// #define _3D   "\xFF\x55\x42\xBD\x00\xFF"
// #define _3L   "\xFF\x55\x44\xBB\x00\xFF"
// #define _3R   "\xFF\x55\x48\xB7\x00\xFF"

// #define _4U   "\xFF\x55\x81\x7E\x00\xFF"
// #define _4D   "\xFF\x55\x82\x7D\x00\xFF"
// #define _4L   "\xFF\x55\x84\x7B\x00\xFF"
// #define _4R   "\xFF\x55\x88\x77\x00\xFF"


// #define STOP  "\xFF\x55\x00\xFF\x00\xFF"

volatile int STOP=FALSE; 

main()
{
  int fd,c, res;
  struct termios oldtio,newtio;
  char buf[255];
/* 
  Open modem device for reading and writing and not as controlling tty
  because we don't want to get killed if linenoise sends CTRL-C.
*/
  fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY ); 
  if (fd <0) {perror(MODEMDEVICE); exit(-1); }

 tcgetattr(fd,&oldtio); /* save current serial port settings */
 bzero(&newtio, sizeof(newtio)); /* clear struct for new port settings */

/* 
  BAUDRATE: Set bps rate. You could also use cfsetispeed and cfsetospeed.
  CRTSCTS : output hardware flow control (only used if the cable has
            all necessary lines. See sect. 7 of Serial-HOWTO)
  CS8     : 8n1 (8bit,no parity,1 stopbit)
  CLOCAL  : local connection, no modem contol
  CREAD   : enable receiving characters
*/
  newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;

/*
  IGNPAR  : ignore bytes with parity errors
  ICRNL   : map CR to NL (otherwise a CR input on the other computer
            will not terminate input)
  otherwise make device raw (no other input processing)
*/
  newtio.c_iflag = IGNPAR | ICRNL;

/*
 Raw output.
*/
 newtio.c_oflag = 0;
 
/*
  ICANON  : enable canonical input
  disable all echo functionality, and don't send signals to calling program
*/
  newtio.c_lflag = ICANON;

/* 
  initialize all control characters 
  default values can be found in /usr/include/termios.h, and are given
  in the comments, but we don't need them here
*/
 newtio.c_cc[VINTR]    = 0;     /* Ctrl-c */ 
 newtio.c_cc[VQUIT]    = 0;     /* Ctrl-\ */
 newtio.c_cc[VERASE]   = 0;     /* del */
 newtio.c_cc[VKILL]    = 0;     /* @ */
 newtio.c_cc[VEOF]     = 4;     /* Ctrl-d */
 newtio.c_cc[VTIME]    = 0;     /* inter-character timer unused */
 newtio.c_cc[VMIN]     = 1;     /* blocking read until 1 character arrives */
 newtio.c_cc[VSWTC]    = 0;     /* '\0' */
 newtio.c_cc[VSTART]   = 0;     /* Ctrl-q */ 
 newtio.c_cc[VSTOP]    = 0;     /* Ctrl-s */
 newtio.c_cc[VSUSP]    = 0;     /* Ctrl-z */
 newtio.c_cc[VEOL]     = 0;     /* '\0' */
 newtio.c_cc[VREPRINT] = 0;     /* Ctrl-r */
 newtio.c_cc[VDISCARD] = 0;     /* Ctrl-u */
 newtio.c_cc[VWERASE]  = 0;     /* Ctrl-w */
 newtio.c_cc[VLNEXT]   = 0;     /* Ctrl-v */
 newtio.c_cc[VEOL2]    = 0;     /* '\0' */

/* 
  now clean the modem line and activate the settings for the port
*/
  tcflush(fd, TCIFLUSH);
  tcsetattr(fd,TCSANOW,&newtio);

/*
  terminal settings done, now handle input
  In this example, inputting a 'z' at the beginning of a line will 
  exit the program.
*/
 while (STOP==FALSE) {     /* loop until we have a terminating condition */
 /* read blocks program execution until a line terminating character is 
    input, even if more than 255 chars are input. If the number
    of characters read is smaller than the number of chars available,
    subsequent reads will return the remaining chars. res will be set
    to the actual number of characters actually read */

    // res = read(fd,buf,255); 
    // buf[res]=0;             /* set end of string, so we can printf */
    // printf(":%s:%d", buf, res);
    // if (buf[0]=='z') STOP=TRUE;
    int  number;
    printf("Type in a number \n");
    scanf("%d", &number);
    printf("Wrote: %d\n", number);
    int n;

    if(number = 1){
      n = write(fd, "ATZ\r", 4);
      if (n < 0)
        fputs("write() of 4 bytes failed!\n", stderr);
    }

    if(number = 2){

      n = write(fd, "ATZ\r", 4);
      if (n < 0)
        fputs("write() of 4 bytes failed!\n", stderr);
    }

}
 /* restore the old port settings */
  tcsetattr(fd,TCSANOW,&oldtio);
}