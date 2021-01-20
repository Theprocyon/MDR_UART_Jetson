#include "MD_COMM_UART.h"
#include <iostream>
using namespace std;

#define RPM 3600

int main(){
	Uart jetson("/dev/pts/5");
	
	
	MD750T TRACK(&jetson, 1);
	MD750T ARM(&jetson, 2);

	//There are Three member functions in MD750T class.
	//MOV_BY_POS, MOV_BY_POS_ABS, Send_Signal.
	
	//MOV_BY_POS is relative position control.
	TRACK.MOV_BY_POS(100, 100); //go forward
	TRACK.MOV_BY_POS(-200, -200); //go backward
	TRACK.MOV_BY_POS(-1500, 1500); // turn

	//MOV_BY_POS_ABS is absolute position control.
	ARM.MOV_BY_POS_ABS(100, RPM, 100, RPM); //park arm at +100 (pulse)
	ARM.MOV_BY_POS_ABS(-200, RPM, -200, RPM); //park arm at -200 (pulse)
	
	
	//Send_Signal is Lowlevel Signal handler.
	TRACK.Send_Signal(PID10_CMD::CMD_ALARM_RESET); // you can send PID10 command using send_signal function.
	TRACK.Send_Signal(PID10_CMD::MAIN_DATA_BC_OFF);
	DATA mydata(1,0x55); // DATA is vector<BYTE>. 
	TRACK.Send_Signal(PID::DEFAULT_SET, mydata); //you can manually send specific PACKET.
	return 0;
}
