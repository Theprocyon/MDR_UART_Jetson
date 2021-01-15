#include "UART.h"
#include <iostream>
using namespace std;

int main(){
	Uart JetsonNano("/dev/pts/4"); // please make UART data transfer loop using socat command
	Uart Driver("/dev/pts/5"); // socat -d -d pty,raw,echo=0 pty,raw,echo=0
	unsigned char Bytes[BUFFERSIZE] = {172,184,183,137,2,170,4,0}; //MDROBOT PROTOCOL - PID 137 SET BAUDRATE 
	JetsonNano.sendUart(Bytes, 8);
	Driver.readUart();
	return 0;
}
