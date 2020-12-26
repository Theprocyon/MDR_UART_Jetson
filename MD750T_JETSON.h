#ifndef _JETSON_MODBUS
#define _JETSON_MODBUS
#include<vector>

//define constant IDs
#define MOTOR_CONTROLLER_MACHINE_ID   183
#define USER_MACHINE_ID               184
#define BROADCAST_ID                  254
#define ENABLE                        1  
#define RETURN_PNT_MAIN_DATA          2   
#define MAX_PACKET_SIZE               30

//Databytes of Command (PID10)
enum PID10_CMD {
	CMD_TQ_OFF = 2,
	CMD_BRAKE = 4,
	MAIN_DATA_BC_ON = 5,
	MAIN_DATA_BC_OFF = 6,
	CMD_ALARM_RESET = 8,
	CMD_POSI_RESET = 10,
	CMD_EMER_ON = 67,
	CMD_EMER_OFF = 68,
};
enum PID_STOP_STATUS {
	STOP_TQ_OFF =     0,
	STOP_SERVO_LOCK = 1,
	STOP_BRAKE =      2,
	STOP_FREE =       3 
};

//define PIDs
#define PID_PNT_MAIN_TOTAL_DATA_NUM   24 
#define PID_MAIN_DATA                 193
#define PID_PNT_POSI_VEL_CMD          206

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
struct POSI_VEL_CMD {
	IBYTE  ID1RPM;
	IBYTE  ID1AMP;
	BYTE   ID1STATE;
	LBYTE  ID1POS;
	IBYTE  ID2RPM;
	IBYTE  ID2AMP;
	BYTE   ID2STATE;
	LBYTE  ID2POS;
};

class MD750T {
private:
	BYTE DEVICE_ID;
	bool      TxPacket(const PACKET&);
	PACKET    RxPacket(void);
	int       RxPacketAnalyzer(BYTE*);
	PACKET    StructPacket(BYTE PID, const DATA&);
public:
	struct POSI_VEL_CMD STATE_DATA;

	MD750T(const char*, BYTE);
	~MD750T();
	int PID1_R_VER_();
	int PID3_C_DEFAULT_SET();
	int PID4_R_REQ_PID_DATA(BYTE PID);
	int PID10_C_SEND_COMMAND(PID10_CMD);
	int PID24_RW_STOP_STATUS(PID_STOP_STATUS);
	int PID49_R_IN_POSITION_OK();
	int PID143_R_VOLT_IN();
	int PID171_RW_IN_POSITION(IBYTE);
	int PID174_C_PNT_TQ_OFF(BYTE,BYTE,BYTE);
	int PID175_C_PNT_BRAKE(BYTE, BYTE, BYTE);
	int PID176_RW_TAR_POSI_VEL(IBYTE);
	int PID196_MONITOR1();
	int PID201_MONITOR2();

	//CONTROL CODE
	int PID206_PNT_POSI_VEL_CMD(POSI_VEL_CMD);
	int PID207_PNT_VEL_CMD(VEL_CMD);
	int PID209_PNT_TQ_CMD(TQ_CMD);

	//Change the value of current Position
	int PID217_POSI_SET1(LBYTE);
	int PID218_POSI_SET2(LBYTE);

	int PID221_MAX_RPM(IBYTE);
};

#endif _JETSON_MODBUS
