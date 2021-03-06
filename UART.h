#ifndef _UART_H
#define _UART_H
#define NSERIAL   256
#define BAUDRATE       B19200
#define BUFFERSIZE     50
#define DEBUG_MODE 0

class Uart {
private:
	const char * uart_target;
	int Port;
public:
	unsigned char rx_data[NSERIAL];
	Uart(const char *);
	bool sendUart(const unsigned char*, int size);
	void readUart();
	void closeUart();

};
#endif
