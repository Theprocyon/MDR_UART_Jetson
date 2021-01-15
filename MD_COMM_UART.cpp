#include "MD_COMM_UART.h"
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

bool MD750T::TxPacket(const PACKET& pct) //incomplete
{
	return false;
}

bool MD750T::RxPacket(void) //incomplete
{
	PACKET RawData(24, 0x00);

	if (RawData.size() == 24 || RawData[0] == USER_MACHINE_ID || RawData[1] == MOTOR_CONTROLLER_MACHINE_ID || CalcChecksum(RawData) == 0) {
		this->STATE_DATA.ID1RPM = Byte2Int(RawData[5], RawData[6]);
		this->STATE_DATA.ID1AMP = Byte2Int(RawData[7], RawData[8]);
		this->STATE_DATA.ID1STATE = RawData[9];
		this->STATE_DATA.ID1POS = Byte2Long(RawData[10], RawData[11], RawData[12], RawData[13]);
		this->STATE_DATA.ID1RPM = Byte2Int(RawData[14], RawData[15]);
		this->STATE_DATA.ID1AMP = Byte2Int(RawData[16], RawData[17]);
		this->STATE_DATA.ID1STATE = RawData[18];
		this->STATE_DATA.ID1POS = Byte2Long(RawData[19], RawData[20], RawData[21], RawData[22]);
	}
	else {
		//fflush
		return false;
	}
}

BYTE MD750T::CalcChecksum(const PACKET& pkt)
{
	BYTE chksent = 0b00000000;
	for (auto i : pkt) chksent += i;
	return (~chksent) + 1;
}


PACKET MD750T::StructPacket(BYTE pid, const DATA& data)
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

MD750T::MD750T(const char* fd, BYTE id)
{
	if (id > 0 || id < 254) this->DEVICE_ID = id;
	else std::cout << "Constructor : Invalid DEVICE_ID" << std::endl;
	//UART Uart(fd);
}

bool MD750T::Send_Signal(PID pid, const DATA& data) {
	return (this->TxPacket(PACKET(StructPacket((BYTE)pid, data))));
}

bool MD750T::Send_Signal(PID pid, BYTE _data_1)
{
	DATA data(1, _data_1);
	return (this->TxPacket(PACKET(StructPacket((BYTE)pid, data))));
}

bool MD750T::Send_Signal(PID pid, BYTE _data_1, BYTE _data_2)
{
	DATA data;
	data.push_back(_data_1);
	data.push_back(_data_2);
	return (this->TxPacket(PACKET(StructPacket((BYTE)pid, data))));
}


bool MD750T::Send_Signal(PID10_CMD cmd)
{
	DATA data(1, (BYTE)cmd);
	return (this->TxPacket(PACKET(StructPacket(10, data))));
}

bool MD750T::MOV_BY_POS(long id1pos, long id2pos)
{
	DATA data(11);
	LBYTE _id1_pos(Long2Byte(id1pos));
	LBYTE _id2_pos(Long2Byte(id2pos));

	data[0] = (id1pos == 0) ? 0 : 1;
	data[1] = _id1_pos.byData1;
	data[2] = _id1_pos.byData2;
	data[3] = _id1_pos.byData3;
	data[4] = _id1_pos.byData4;
	data[5] = (id2pos == 0) ? 0 : 1;
	data[6] = _id2_pos.byData1;
	data[7] = _id2_pos.byData2;
	data[8] = _id2_pos.byData3;
	data[9] = _id2_pos.byData4;
	data[10] = RETURN_PNT_MAIN_DATA;

	return Send_Signal(PID::PNT_INC_POSI_CMD, data);
}

bool MD750T::MOV_BY_POS_ABS(long id1pos, int id1spd, long id2pos, int id2spd)
{
	DATA data(15);

	IBYTE _id1_spd(Int2Byte(id1spd));
	IBYTE _id2_spd(Int2Byte(id2spd));
	LBYTE _id1_pos(Long2Byte(id1pos));
	LBYTE _id2_pos(Long2Byte(id2pos));

	data[0] = ENABLE;
	data[1] = _id1_pos.byData1;
	data[2] = _id1_pos.byData2;
	data[3] = _id1_pos.byData3;
	data[4] = _id1_pos.byData4;
	data[5] = _id1_spd.byLow;
	data[6] = _id1_spd.byHigh;
	data[7] = ENABLE;
	data[8] = _id2_pos.byData1;
	data[9] = _id2_pos.byData2;
	data[10] = _id2_pos.byData3;
	data[11] = _id2_pos.byData4;
	data[12] = _id2_spd.byLow;
	data[13] = _id2_spd.byHigh;
	data[14] = RETURN_PNT_MAIN_DATA;

	return Send_Signal(PID::PNT_POSI_VEL_CMD, data);
}

