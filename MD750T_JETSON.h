#ifndef _MD_COMM_UART
#define _MD_COMM_UART
#include<vector>
#include<iostream>

//define constant IDs
#define ENABLE                        1  
#define RETURN_PNT_MAIN_DATA          2   
#define MAX_PACKET_SIZE               30
#define MOTOR_CONTROLLER_MACHINE_ID   183
#define USER_MACHINE_ID               184
#define BROADCAST_ID                  254
#define DEFAULT_WRITE_CHK		      0x55
#define WRITE_CHK		              0xaa
//PIDs
enum class PID {
	//1byte data
	VER = 1,
	DEFAULT_SET = 3,
	REQ_PID_DATA = 4,
	COMMAND = 10,
	STOP_STATUS = 24,
	IN_POSITION_OK = 49,
	//2byte data
	VOLT_IN = 143,
	IN_POSITION = 171,
	PNT_TQ_OFF = 174,
	PNT_BRAKE = 175,
	MONITOR1 = 196,
	MONITOR2 = 201,

	//CTRLCODE
	PNT_POSI_VEL_CMD = 206,
	PNT_VEL_CMD = 207,
	PNT_TQ_CMD = 209,

	//Change the value of current position
	POSI_SET1 = 217,
	POSI_SET2 = 218,

	MAX_RPM = 221
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

enum class PID_STOP_STATUS {
	STOP_TQ_OFF = 0,
	STOP_SERVO_LOCK = 1,
	STOP_BRAKE = 2,
	STOP_FREE = 3
};

//define data structure; BYTE WORD
typedef unsigned char 	   BYTE;
typedef unsigned int 	   WORD;
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
int Byte2Int(BYTE byLow, BYTE byHigh)
{
	return (byLow | (int)byHigh << 8);
}
long Byte2Long(BYTE byData1, BYTE byData2, BYTE byData3, BYTE byData4)
{
	return ((long)byData1 | (long)byData2 << 8 | (long)byData3 << 16 |
		(long)byData4 << 24);
}
IBYTE Int2Byte(int nIn)
{
	IBYTE Ret;

	Ret.byLow = nIn & 0xff;
	Ret.byHigh = nIn >> 8 & 0xff;
	return Ret;
}
LBYTE Long2Byte(long lIn)
{
	LBYTE Ret;

	Ret.byData1 = lIn & 0xff;
	Ret.byData2 = lIn >> 8 & 0xff;
	Ret.byData3 = lIn >> 16 & 0xff;
	Ret.byData4 = lIn >> 24 & 0xff;
	return Ret;
}

//structure of PID206 data
struct POSI_VEL_CMD {
	BYTE  ID1ENABLE = 0;
	LBYTE ID1POS = Long2Byte(0);
	IBYTE ID1MAXSPD = Int2Byte(0);
	BYTE  ID2ENABLE = 0;
	LBYTE ID2POS = Long2Byte(0);
	IBYTE ID2MAXSPD = Int2Byte(0);
	BYTE  PID_MONITOR_ID = 0;
};
struct VEL_CMD { //RPM
	BYTE  ID1ENABLE = 0;
	IBYTE ID1SPD = Int2Byte(0);
	BYTE  ID2ENABLE = 0;
	IBYTE ID2SPD = Int2Byte(0);
	BYTE  PID_MONITOR_ID = 0;
};
struct TQ_CMD { //Torque(-1023~1023)
	BYTE  ID1ENABLE = 0;
	IBYTE ID1TQ = Int2Byte(0);
	BYTE  ID2ENABLE = 0;
	IBYTE ID2TQ = Int2Byte(0);
	BYTE  PID_MONITOR_ID = 0;
};

//define ctrlstruct to data
struct STATE {
	IBYTE  ID1RPM;
	IBYTE  ID1AMP;
	BYTE   ID1STATE;
	LBYTE  ID1POS;
	IBYTE  ID2RPM;
	IBYTE  ID2AMP;
	BYTE   ID2STATE;
	LBYTE  ID2POS;
};

class MDT {
private:
	BYTE      DEVICE_ID = BROADCAST_ID;
	//UART Uart(POS);
	bool      TxPacket(const PACKET&);
	PACKET    RxPacket(void);
	int       RxPacketAnalyzer(PACKET);
	BYTE      CalcChecksum(const PACKET&);
	PACKET    StructPacket(BYTE pid, const DATA&);
public:
	static STATE STATE_DATA;

	MDT(const char*, BYTE);
	~MDT();

	bool Send_Signal(PID pid, const DATA&);
};

#endif _MD_COMM_UART
