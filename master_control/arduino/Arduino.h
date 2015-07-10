#ifndef ARDUINO_H
#define ARDUINO_H

#include <SerialStream.h>
#include <SerialStreamBuf.h>
#include <SerialPort.h>
// #include <string>

using namespace std;

class Arduino{
public:
    Arduino();
    int open();
    DataBuffer read();
    void close();
private:
    string dev = "/dev/ttyACM0";
    SerialPort serial;
};

#endif // ARDUINO_H