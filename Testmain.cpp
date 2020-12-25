#include "UART.h"

int main(){
	Uart Test("/dev/ttyTHS1");
	unsigned char message = 1;
	Test.sendUart(&message);
	return 0;
}
