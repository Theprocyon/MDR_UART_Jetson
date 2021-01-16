#ifndef MD_COMM_UART
#define MD_COMM_UART
#include<vector>
#include<iostream>
#include"UART.h"
//define constant IDs
#define ENABLE                        1  
#define RETURN_PNT_MAIN_DATA          2   
#define MAX_PACKET_SIZE               30
#define MOTOR_CONTROLLER_MACHINE_ID   184
#define USER_MACHINE_ID               172
#define BROADCAST_ID                  254
#define DEFAULT_WRITE_CHK		      0x55
#define WRITE_CHK		              0xaa
#define DONT_CARE                     0x00
//PIDs
enum class PID {
	//1byte data
	VER = 1, //R
	DEFAULT_SET = 3,
	REQ_PID_DATA = 4,
	ACK = 7, //R
	COMMAND = 10,
	ALARM_RESET = 12,
	POSI_RESET = 13,
	STOP_STATUS = 24, //RW
	IN_POSITION_OK = 49,//R

	//2byte data
	VOLT_IN = 143, //R
	RETURN_TYPE = 149, //RW
	IN_POSITION = 171, //R
	PNT_TQ_OFF = 174,
	PNT_BRAKE = 175,
	TAR_POSI_VEL = 176,
	MONITOR1 = 196, //R
	MONITOR2 = 201, //R

	//CTRLCODE
	PNT_POSI_VEL_CMD = 206,
	PNT_VEL_CMD = 207,
	PNT_TQ_CMD = 209,
	PNT_MAIN_DATA = 210, //R
	PNT_INC_POSI_CMD = 215,
	//Change the value of current position

	POSI_SET1 = 217,
	POSI_SET2 = 218,
	MAX_RPM = 221 //RW
};

//Databytes of Command (PID10)
enum class PID10_CMD {
	CMD_TQ_OFF = 2,
	CMD_BRAKE = 4,
	MAIN_DATA_BC_ON = 5,
	MAIN_DATA_BC_OFF = 6,
	CMD_ALARM_RESET = 8,
	CMD_POSI_RESET = 10,
	CMD_EMER_ON = 67,
	CMD_EMER_OFF = 68,
};

//define data structure; BYTE WORD
typedef unsigned char 	   BYTE;
typedef unsigned int 	   WORD;
//PACKET and DATA is vector of BYTE
typedef std::vector<BYTE>  PACKET;
typedef std::vector<BYTE>  DATA;
//INT(16bit) for 8bit CPU
typedef struct {
	BYTE byLow;
	BYTE byHigh;
} IBYTE;
//LONG(32bit) for 8bit CPU
typedef struct {
	BYTE byData1;	// Low Byte
	BYTE byData2;
	BYTE byData3;
	BYTE byData4;	// High Byte
} LBYTE;

// define BYTE - INT - LONG converter
int Byte2Int(BYTE, BYTE);
long Byte2Long(BYTE, BYTE, BYTE, BYTE);
IBYTE Int2Byte(int);
LBYTE Long2Byte(long);

//define ctrlstruct to data
struct STATE {
	int  ID1RPM = 0;
	int  ID1AMP = 0;
	BYTE   ID1STATE = 0;
	long  ID1POS = 0;

	int  ID2RPM = 0;
	int  ID2AMP = 0;
	BYTE   ID2STATE = 0;
	long  ID2POS = 0;
};

class MD750T {
private:
	Uart*     uart;
	BYTE      DEVICE_ID = BROADCAST_ID;
	STATE     DEVICE_STATE = { 0 };
	bool      TxPacket(const PACKET&);
	bool      RxPacket(void);
	BYTE      CalcChecksum(const PACKET&);
	PACKET    StructPacket(BYTE pid, const DATA&);

public:

	//lowlevel control
	MD750T(Uart*, BYTE);
	bool Send_Signal(PID pid, const DATA&);
	bool Send_Signal(PID pid, BYTE);
	bool Send_Signal(PID pid, BYTE, BYTE);
	bool Send_Signal(PID10_CMD);
	
	//highlevel control
	bool MOV_BY_POS(long, long);
	bool MOV_BY_POS_ABS(long, int, long, int);
};

#endif
