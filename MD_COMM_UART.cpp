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
	PACKET Packet(6 + data.size());
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

bool MDT::Send_Signal(PID pid, const DATA& data) //send 1 or 2 byte signal. use PID:: and vector<BYTE>for parameter;
{
	PACKET* _packet_to_send;
	_packet_to_send = new PACKET(StructPacket((BYTE)pid, data));

	//tx
	return true;
}
