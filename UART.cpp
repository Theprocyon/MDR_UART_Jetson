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

	usleep(500000);  // 1 sec delay

	if (Port == -1)
	{
		printf("Alert from UART.cpp : Unable to open port.");
	}

	//8 data bits, 1 stop bit, no parity, 57600bps, from MDRobot Communication specification
	
    Port_options.c_cflag &= ~PARENB;            // Disables the Parity   
    Port_options.c_cflag &= ~CSTOPB;            // 1 Stop bit 
    Port_options.c_cflag &= ~CSIZE;	            
    Port_options.c_cflag |=  CS8;               // Set the data bits = 8                                 	 
    Port_options.c_cflag &= ~CRTSCTS;           // No Hardware flow Control                         
    Port_options.c_cflag |=  CREAD | CLOCAL;                  // Enable receiver,Ignore Modem Control lines       
    Port_options.c_iflag &= ~(IXON | IXOFF | IXANY);          // Disable XON/XOFF flow control both input & output
    Port_options.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);  // Non Cannonical mode                            
    Port_options.c_oflag &= ~OPOST;                           // No Output Processing
    Port_options.c_lflag = 0;               //  enable raw input
    Port_options.c_cc[VTIME] = 0;           // Wait indefinetly
    cfsetispeed(&Port_options,BAUDRATE);    // Set Read  Speed 
    cfsetospeed(&Port_options,BAUDRATE);    // Set Write Speed 
	
    int attributes = tcsetattr(Port, TCSANOW, &Port_options); //Save Attributes

	if (attributes != 0) printf("\nAlert from UART.cpp : attribute failed.\n");
	else printf("\nAlert from UART.cpp : Initializing Port Complete : %s\n",Pt);

	tcflush(Port, TCIFLUSH);
	tcflush(Port, TCIOFLUSH);

	usleep(500000);   // 0.5 sec delay
}

bool Uart::sendUart(const unsigned char* data, int size) {
	int wrotelen = 0;
	int failcnt = 0;
    while(wrotelen < size && failcnt < 10)
    {  
        int ret = 0;
        int towritelen = size - wrotelen;
        unsigned char *ptr = const_cast<unsigned char*>(data) + wrotelen;
        ret = write(Port,ptr,towritelen*sizeof(unsigned char));

        if (ret > 0) wrotelen += ret;
        else failcnt++;
    }
	
	for(int i = 0; i < size; i++)
	printf("%d ",data[i]);
	cout<<endl;
    return true;
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
	for (int i = packet_length; i < NSERIAL; i++) this->rx_data[i] = { '\0' }; //Fill extra space \0
}

void Uart::closeUart() {
	close(Port);
}
