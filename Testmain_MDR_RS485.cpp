#include "MD_COMM_UART.h"
#define RPM 3600


int main() {

	MD750T TRACK(nullptr, 1);
	MD750T ARM(nullptr, 2);

	//MOV_BY_POS는 상대위치 컨트롤.
	TRACK.MOV_BY_POS(100, 100); //현재 위치로부터 100만큼 앞으로 이동
	TRACK.MOV_BY_POS(-200, -200); //도착한 위치로부터 200만큼 뒤로 이동(초기로부터 -100)
	TRACK.MOV_BY_POS(-1500, 1500); // 회전

	//MOV_BY_POS_ABS는 절대 위치 컨트롤. 
	ARM.MOV_BY_POS_ABS(100, RPM, 100, RPM); //로봇팔을 100에 위치시킴(각도는 계산필요)
	ARM.MOV_BY_POS_ABS(-200, RPM, -200, RPM); //로봇팔을 -200에 위치시킴(각도는 계산필요)

	return 0;
}
