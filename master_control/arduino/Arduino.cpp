# include <Arduino.h>

Arduino::Arduino(){
    serial(dev);
}

int Arduino::abrir(){
    int estado = 0;

    serial.Open(SerialPort::BAUD_9600,
                SerialPort::CHAR_SIZE_8,
                SerialPort::PARITY_NONE,
                SerialPort::STOP_BITS_1,
                SerialPort::FLOW_CONTROL_NONE);

    if (serial.IsOpen() == false)
        estado = -1;

    return estado;
}

void Arduino::cerrar(){
    serial.Close();
}

DataBuffer Arduino::leer(){
    SerialPort::DataBuffer buffer;
    serial.Read(buffer, 10, 1000);

    return buffer;
}