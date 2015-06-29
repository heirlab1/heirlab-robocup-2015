#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "tx_serial.h"

// was unsigned
char gbRcvPacket[6];
unsigned char gbRcvPacketNum;
unsigned short gwRcvData;
unsigned char gbRcvFlag;

int rx_check(int fd)
{

	int i, j, RcvNum, res;
	unsigned char checksum;
	// char buf[255];


	if(gbRcvFlag == 1)
		return 1;

	// Fill packet buffer
	if(gbRcvPacketNum < 6)
	{
		RcvNum = hal_rx(fd, &gbRcvPacket[gbRcvPacketNum], (6 - gbRcvPacketNum) );
		if( RcvNum != -1 )
			gbRcvPacketNum += RcvNum;
	}

	printf("got here weirdo\n");

	// Find header
	if(gbRcvPacketNum >= 2)
	{
		for( i=0; i<gbRcvPacketNum; i++ )
		{
			if(gbRcvPacket[i] == 0xff)
			{
				if(i <= (gbRcvPacketNum - 2))
				{
					if(gbRcvPacket[i+1] == 0x55)
						break;
				}
			}
		}

		if(i > 0)
		{
			if(i == gbRcvPacketNum)
			{
				// Can not find header
				if(gbRcvPacket[i - 1] == 0xff)
					i--;
			}

			// Remove data before header
			for( j=i; j<gbRcvPacketNum; j++)
			{
				gbRcvPacket[j - i] = gbRcvPacket[j];
			}
			gbRcvPacketNum -= i;
		}
	}



	// Verify packet
	if(gbRcvPacketNum == 6)
	{
		if(gbRcvPacket[0] == 0xff && gbRcvPacket[1] == 0x55)
		{
			checksum = ~gbRcvPacket[3];
			if(gbRcvPacket[2] == checksum)
			{
				checksum = ~gbRcvPacket[5];
				if(gbRcvPacket[4] == checksum)
				{
					gwRcvData = (unsigned short)((gbRcvPacket[4] << 8) & 0xff00);
					gwRcvData += gbRcvPacket[2];
					gbRcvFlag = 1;
				}
			}
		}

		gbRcvPacket[0] = 0x00;
		gbRcvPacketNum = 0;
	}

	return gbRcvFlag;
}

int rx_data(){
	gbRcvFlag = 0;
	return (int)gwRcvData;
}

int hal_rx(int fd, unsigned char *pPacket, int numPacket ){
	printf("%d\n", numPacket);
	printf("%d\n", fd);
	memset(pPacket, 0, numPacket);
	printf("hallin\n");
	return read(fd, pPacket, numPacket);
}