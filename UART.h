#ifndef _UART_H
#define _UART_H

const char* uart_target = "/dev/ttyTHS1";
#define NSERIAL   256
#define BAUDRATE       B57600 //50ms
#define BUFFERSIZE     50

class Uart {
private:
	int Port;
public:
	unsigned char rx_data[NSERIAL];
	Uart();
	bool sendUart(unsigned char* msg);
	void readUart();
	void closeUart();

};
#endif