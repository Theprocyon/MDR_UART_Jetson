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
int Byte2Int(BYTE,BYTE);
long Byte2Long(BYTE,BYTE,BYTE,BYTE);
IBYTE Int2Byte(int);
LBYTE Long2Byte(long);

//structure of PID206 data
struct POSI_VEL_CMD {
	BYTE  ID1ENABLE = 0;
	long ID1POS = 0;
	int ID1MAXSPD = 0;
	BYTE  ID2ENABLE = 0;
	long ID2POS = 0;
	int ID2MAXSPD = 0;
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
	PACKET Send_Signal(PID pid, const DATA&);
	PACKET Send_Signal(PID pid, BYTE);
	PACKET Send_Signal(PID pid, BYTE, BYTE);
	PACKET Send_Signal(PID pid, PID10_CMD);
	PACKET Send_Signal(PID pid, POSI_VEL_CMD);
};

#endif _MD_COMM_UART
