#ifndef _TX_SERIAL_HEADER
#define _TX_SERIAL_HEADER


#ifdef __cplusplus
extern "C" {
#endif

//	Map commands to RC-100 button key values
/*
	calculate more inputs by combining buttons.
	key values are the value the buttons added key values
	for example: R = 8, _U = 1, so _UR = 9
*/
#define FORWARD		(1)		// RC100 Button: U
#define BACKWARD	(2) 	// RC100 Button: D
#define command3	(4) 	// RC100 Button: L
#define command4	(8) 	// RC100 Button: R
#define command6	(16) 	// RC100 Button: 1
#define command7	(32) 	// RC100 Button: 2
#define command8	(64) 	// RC100 Button: 3
#define command9	(128) 	// RC100 Button: 4
#define command10	(256) 	// RC100 Button: 5
#define command11	(512) 	// RC100 Button: 6


#ifdef __cplusplus
}
#endif

#endif
