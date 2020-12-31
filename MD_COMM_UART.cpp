#include "MD_COMM_UART.h"

bool MDT::TxPacket(const PACKET&)
{
	return false;
}

PACKET MDT::RxPacket(void)
{
	return PACKET();
}

int MDT::RxPacketAnalyzer(PACKET rcv)
{
	if (CalcChecksum(rcv) != 0) return -1;

	return 0;
}

BYTE MDT::CalcChecksum(const PACKET& pkt)
{
	BYTE chksent = 0b00000000;
	for (auto i : pkt) chksent += i;
	return (~chksent) + 1;
}

PACKET MDT::StructPacket(BYTE pid, const DATA& data)
{
	PACKET Packet;
	Packet.push_back(MOTOR_CONTROLLER_MACHINE_ID); //RMID
	Packet.push_back(USER_MACHINE_ID);             //PMID
	Packet.push_back(this->DEVICE_ID);             //DEV_ID
	Packet.push_back(pid);                         //PID
	Packet.push_back(data.size());                 //DataNumber
	for (auto i : data) 	Packet.push_back(i);   //DATA
	Packet.push_back(this->CalcChecksum(Packet));  //CHK
	return Packet;
}

MDT::MDT(const char* fd, BYTE id)
{
	if (id > 0 || id < 254) this->DEVICE_ID = id;
	else std::cout << "Constructor : Invalid DEVICE_ID" << std::endl;
	//UART Uart(fd);
}

MDT::~MDT()
{
}

PACKET MDT::Send_Signal(PID pid, const DATA& data) {
	return PACKET(StructPacket((BYTE)pid, data));
}

PACKET MDT::Send_Signal(PID pid, BYTE _data_1)
{
	DATA data(1, _data_1);
	return PACKET(StructPacket((BYTE)pid, data));
}

PACKET MDT::Send_Signal(PID pid, BYTE _data_1, BYTE _data_2)
{
	DATA data;
	data.push_back(_data_1);
	data.push_back(_data_2);
	return PACKET(StructPacket((BYTE)pid, data));
}


PACKET MDT::Send_Signal(PID pid, PID10_CMD cmd)
{
	DATA data(1, (BYTE)cmd);
	return PACKET(StructPacket((BYTE)pid, data));
}

PACKET MDT::Send_Signal(PID pid, POSI_VEL_CMD str)
{
	DATA data(15);

	LBYTE POS1 = Long2Byte(str.ID1POS);
	IBYTE SPD1 = Int2Byte(str.ID1MAXSPD);
	LBYTE POS2 = Long2Byte(str.ID2POS);
	IBYTE SPD2 = Int2Byte(str.ID2MAXSPD);

	data[0] = str.ID1ENABLE;
	data[1] = POS1.byData1;
	data[2] = POS1.byData2;
	data[3] = POS1.byData3;
	data[4] = POS1.byData4;
	data[5] = SPD1.byLow;
	data[6] = SPD1.byHigh;
	data[7] = str.ID2ENABLE;
	data[8] = POS2.byData1;
	data[9] = POS2.byData2;
	data[10] = POS2.byData3;
	data[11] = POS2.byData4;
	data[12] = SPD2.byLow;
	data[13] = SPD2.byHigh;
	data[14] = str.PID_MONITOR_ID;

	return PACKET(StructPacket((BYTE)pid, data));
}

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
