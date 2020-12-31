#include "MD_COMM_UART.h"

void PrintPacket(const PACKET& pct) {
	for (auto i : pct)
	printf("%d ", i);
	printf("\n");
	return;
}
int main() {
	MDT MD750T_TRACK(nullptr, 1);
	MDT MD750T_ARM(nullptr, 2);
	//class MDT is Motercontroller of MDROBOT.
	//Parameters of MDT(const char* UART PORT PATH,BYTE DEVICE ID)
	MD750T_TRACK.Send_Signal(PID::ALARM_RESET, DONT_CARE);

	//you can manually send signal any PID you want
	MD750T_TRACK.Send_Signal(PID::ALARM_RESET, DONT_CARE);
	MD750T_TRACK.Send_Signal(PID::STOP_STATUS, 1); //servo lock

	//in the case of the COMMAND pid, you can use PID10_CMD type as parameter.
	MD750T_TRACK.Send_Signal(PID::COMMAND, PID10_CMD::CMD_POSI_RESET);
	MD750T_TRACK.Send_Signal(PID::COMMAND, PID10_CMD::CMD_BRAKE);
	MD750T_TRACK.Send_Signal(PID::COMMAND, PID10_CMD::CMD_ALARM_RESET);

	//you can use just number as parameter (NOT RECOMMENDED)
	MD750T_TRACK.Send_Signal((PID)10, (BYTE)10);

	//Control code, pid 206 receives struct POSI_VEL_CMD
	POSI_VEL_CMD Position;

	Position.ID1ENABLE = 1;
	Position.ID1MAXSPD = 100;
	Position.ID1POS = 1024;
	Position.ID2ENABLE = 1;
	Position.ID2MAXSPD = 100;
	Position.ID2POS = 1024;
	Position.PID_MONITOR_ID = RETURN_PNT_MAIN_DATA;

	MD750T_TRACK.Send_Signal(PID::PNT_POSI_VEL_CMD, Position); //position control sent
	Position.ID1POS += 1024;
	MD750T_TRACK.Send_Signal(PID::PNT_POSI_VEL_CMD, Position); //position control(motor 1's position += 1024) sent
	return 0;
}
