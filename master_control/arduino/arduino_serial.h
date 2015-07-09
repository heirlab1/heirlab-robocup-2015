#ifndef ARDUINO_H
#define ARDUINO_H

#include <SerialStream.h>
#include <SerialStreamBuf.h>
#include <SerialPort.h>
#include <string>

class Arduino{
public:
    Arduino();
    int open);
    DataBuffer read();
    void close();
private:
    string dev = "/dev/ACM0";
    SerialPort serial;
};

#endif // ARDUINO_H