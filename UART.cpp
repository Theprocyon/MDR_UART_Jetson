#include "UART.h"
#include <iostream>
#include <unistd.h>
#include <sys/fcntl.h>
#include <termios.h>
#include <string>

using namespace std;

Uart::Uart(const char * Pt) {
	uart_target = Pt;
	struct termios  Port_options;   

	tcgetattr(Port, &Port_options);	

	Port = open(uart_target, O_RDWR | O_NOCTTY);

	tcflush(Port, TCIFLUSH);
	tcflush(Port, TCIOFLUSH);

	usleep(1000000);  // 1 sec delay

	if (Port == -1)
	{
		printf("포트 오픈 불가능.");
	}

	//MD로봇 통신사양서 참고 : 8 data bits, 1 stop bit, no parity, 57600bps
    Port_options.c_cflag &= ~PARENB;            // Disables the Parity Enable bit(PARENB),So No Parity   
    Port_options.c_cflag &= ~CSTOPB;            // CSTOPB = 2 Stop bits,here it is cleared so 1 Stop bit 
    Port_options.c_cflag &= ~CSIZE;	            // Clears the mask for setting the data size             
    Port_options.c_cflag |=  CS8;               // Set the data bits = 8                                 	 
    Port_options.c_cflag &= ~CRTSCTS;           // No Hardware flow Control                         
    Port_options.c_cflag |=  CREAD | CLOCAL;                  // Enable receiver,Ignore Modem Control lines       
    Port_options.c_iflag &= ~(IXON | IXOFF | IXANY);          // Disable XON/XOFF flow control both input & output
    Port_options.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);  // Non Cannonical mode                            
    Port_options.c_oflag &= ~OPOST;                           // No Output Processing
    Port_options.c_lflag = 0;               //  enable raw input instead of canonical,
    Port_options.c_cc[VTIME] = 0;           // Wait indefinetly 
    cfsetispeed(&Port_options,BAUDRATE);    // Set Read  Speed 
    cfsetospeed(&Port_options,BAUDRATE);    // Set Write Speed 
    int attributes = tcsetattr(Port, TCSANOW, &Port_options);

	if (attributes != 0) printf("\nERROR");
	else printf("\n포트 초기화 완료.\n");

	tcflush(Port, TCIFLUSH);
	tcflush(Port, TCIOFLUSH);

	usleep(500000);   // 0.5 sec delay
}

bool Uart::sendUart(unsigned char* msg) {

	unsigned char tx_buffer[BUFFERSIZE];
	unsigned char* p_tx_buffer;

	p_tx_buffer = &tx_buffer[0];

	for (int i = 0; i < BUFFERSIZE; i++) {
		*p_tx_buffer++ = msg[i];
	}

	if (Port != -1)
	{
		int count = write(Port, tx_buffer, (p_tx_buffer - &tx_buffer[0]));

		usleep(1000);   // .001 sec delay

		printf("Num of Packets = %d\n", count);

		if (count < 0)
		{
			printf("Failed\n");
			return false;
		}
		return true;
	}
	else return false;

	usleep(1000);

}
void Uart::readUart() {
	unsigned char rx_buffer[BUFFERSIZE];
	bool          onread = true;
	int           packet_length;

	usleep(1000);   // .001 sec delay
	
	packet_length = read(Port, rx_buffer, BUFFERSIZE);

	for (int i = 0; i < packet_length; i++) {
		printf("%x ", rx_buffer[i]);
		this->rx_data[i] = rx_buffer[i];
	}
	for (int i = packet_length; i < NSERIAL; i++) this->rx_data[i] = { '\0' };
}

void Uart::closeUart() {
	close(Port);
}
