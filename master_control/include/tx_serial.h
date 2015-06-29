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

// int rx_check(int fd);
// int rx_data();
// int hal_rx(int fd, unsigned char *pPacket, int numPacket);


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

//  			--- Walking mapping ---
// 		command				tx#		// RC100 Button
#define GOFORWARD			(1)		// U
#define GOBACKWARD			(2) 	// D
#define TURNLEFT			(4) 	// L
#define TURNRIGHT			(8) 	// R
#define GOFORWARDTURNLEFT	(5)		// U+L
#define GOFORWARDTURNRIGHT	(9)		// U+R
#define GOLEFT				(260)	// L+5
#define GORIGHT				(264)	// R+5
#define GOFORWARDLEFT 		(261)	// L+U+5
#define GOFORWARDRIGHT 		(265)	// R+U+5
#define GOBACKWARDLEFT 		(262)	// L+D+5
#define GOBACKWARDRIGHT		(266)	// R+D+5

//  			--- Kick mapping ---
// 		command				tx#		// RC100 Button
#define LEFTKICKFORWARD 	(33)	// 2+U
#define RIGHTKICKFORWARD 	(129)	// 4+U
#define LEFTKICKHARD		(545)	// 2+6+U
#define RIGHTKICKHARD 		(641)	// 4+6+U
#define LEFTKICKBACKWARD 	(34) 	// 2+D
#define RIGHTKICKBARDWARD 	(130)	// 4+D
#define LEFTKICKLEFT 		(36) 	// 2+L
#define RIGHTKICKLEFT 		(132) 	// 4+L
#define LEFTKICKRIGHT 		(40) 	// 2+R
#define RIGHTKICKRIGHT 		(136) 	// 4+R

//  			--- Block mapping ---
// 		command				tx#		// RC100 Button
#define PREPAREBLOCK 		(64) 	// 3
#define BLOCKFRONT 			(65) 	// 3+U
#define BLOCKLEFT 			(68) 	// 3+L
#define BLOCKRIGHT 			(72) 	// 3+R

//  			--- Misc mapping ---
// 		command				tx#		// RC100 Button
#define TORQUEOFF 			(834) 	// 5+6+D+3
#define TORQUEON 			(785) 	// 5+6+U+1
#define STANDUPCHEST 		(17) 	// 1+U
#define STANDUPBACK 		(18) 	// 1+D
#define BEATCHEST 			(20) 	// 1+L
#define CHEER 				(24) 	// 1+R

// 				--- Reference buttons ---
// 		command				tx#		// RC100 Button
#define button1				(16) 	// 1
#define button2				(32) 	// 2
#define button3				(64) 	// 3
#define button4				(128) 	// 4
#define button5				(256) 	// 5
#define button6				(512) 	// 6


#ifdef __cplusplus
}
#endif

#endif
