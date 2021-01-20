#include "MD_COMM_UART.h"
#include <iostream>
using namespace std;

#define RPM 3600

int main(){
	Uart jetson("/dev/pts/5");
	
	MD750T TRACK(&jetson, 1);
	MD750T ARM(&jetson, 2);

	//MOV_BY_POS
	TRACK.MOV_BY_POS(100, 100); //go forward
	TRACK.MOV_BY_POS(-200, -200); //go backward
	TRACK.MOV_BY_POS(-1500, 1500); // turn

	//MOV_BY_POS_ABS is absolute position control.
	ARM.MOV_BY_POS_ABS(100, RPM, 100, RPM); //park arm at +100 (pulse)
	ARM.MOV_BY_POS_ABS(-200, RPM, -200, RPM); //park arm at -200 (pulse)
	return 0;
}
