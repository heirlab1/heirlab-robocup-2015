#ifndef _TX_SERIAL_HEADER
#define _TX_SERIAL_HEADER


#ifdef __cplusplus
extern "C" {
#endif

// communication methods
int tx_data(int fd, int command_key);
int encode_data(int fd, int data);
int write_tx(int fd, unsigned char *pPacket, int numPacket);

int open_port();

int rx_check(int fd);
int rx_data();
int hal_rx();


/* baudrate settings are defined in <asm/termbits.h>, which is
included by <termios.h> */
#define BAUDRATE B57600            

/* change this definition for the correct port */
#define MODEMDEVICE "/dev/ttyUSB0"

#define _POSIX_SOURCE 1 /* POSIX compliant source */

#define FALSE 0
#define TRUE 1

#define DEFAULT_DEVICEINDEX 0
#define TIMEOUT_TIME    1000 // msec


#define FORWARD		(1)		// RC100 Button: U
#define BACKWARD	(2) 	// RC100 Button: D
#define TURNLEFT	(4) 	// RC100 Button: L
#define TURNRIGHT	(8) 	// RC100 Button: R
#define KICK		(16) 	// RC100 Button: 1
#define command7	(32) 	// RC100 Button: 2
#define command8	(64) 	// RC100 Button: 3
#define command9	(128) 	// RC100 Button: 4
#define command10	(256) 	// RC100 Button: 5
#define command11	(512) 	// RC100 Button: 6


#ifdef __cplusplus
}
#endif

#endif
