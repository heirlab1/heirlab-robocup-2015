#include &lt SerialStream.h &gt
#include &lt iostream &gt
#define PORT "/dev/cu.usbmodem1411 " //This is system-specific

SerialStream ardu;

using namespace std;
using namespace LibSerial;

void open(){
    /*The arduino must be setup to use the same baud rate*/ 
    ardu.Open(PORT);    
    ardu.SetBaudRate(SerialStreamBuf::BAUD_9600);  
    ardu.SetCharSize(SerialStreamBuf::CHAR_SIZE_8);
}

int get(char out){    
    int res;
    char str[SOME_BIG_SIZE];
    ardu << out;
    ardu >> str;
    sscanf(str,"%d",&res);
    return res;
}