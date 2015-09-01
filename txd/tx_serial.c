#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "tx_serial.h"

// Returns true if the data was successfully transmitted
int tx_data(int fd, int command_key){
  if (encode_data(fd, command_key)){
   return 1;
  }
  else {
    return 0;
  }

}

// Encodes data so the CM-700 can understand it
// Immitates an RC-100 controller
int encode_data(int fd, int data){
  unsigned char SndPacket[6];
  unsigned short word = (unsigned short)data;
  unsigned char lowbyte = (unsigned char)(word & 0xff);
  unsigned char highbyte = (unsigned char)((word >> 8) & 0xff);

  SndPacket[0] = 0xff;
  SndPacket[1] = 0x55;
  SndPacket[2] = lowbyte;
  SndPacket[3] = ~lowbyte;
  SndPacket[4] = highbyte;
  SndPacket[5] = ~highbyte;

  if(write_tx(fd, SndPacket, 6 ) != 6 )
    return 0;

  return 1;
}

// Writes the encoded data over the serial port
int write_tx(int fd, unsigned char *pPacket, int numPacket)
{
  return write(fd, pPacket, numPacket);
}
