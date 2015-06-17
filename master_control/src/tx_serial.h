#ifndef _TX_SERIAL_HEADER
#define _TX_SERIAL_HEADER


#ifdef __cplusplus
extern "C" {
#endif

// communication method
// returns 1 on success, 0 on failure
int tx_data(int command_key);


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


#ifdef __cplusplus
}
#endif

#endif
