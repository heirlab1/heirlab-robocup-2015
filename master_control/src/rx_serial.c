#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "tx_serial.h"

int rx_check()
{
	int RcvNum;
	unsigned char checksum;
	int i, j;

	if(gbRcvFlag == 1)
		return 1;

	// Fill packet buffer
	if(gbRcvPacketNum < 6)
	{
		RcvNum = zgb_hal_rx( &gbRcvPacket[gbRcvPacketNum], (6 - gbRcvPacketNum) );
		if( RcvNum != -1 )
			gbRcvPacketNum += RcvNum;
	}

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

int rx_data()
{
	gbRcvFlag = 0;
	return (int)gwRcvData;
}

int zgb_hal_rx( unsigned char *pPacket, int numPacket )
{
	memset(pPacket, 0, numPacket);
	return read(gSocket_fd, pPacket, numPacket);
}