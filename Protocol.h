#pragma once
#include <Windows.h>
enum PacketList
{
	P_CONNECTIONSUCCESS_ACK = 100,

	P_TESTPACKET1_REQ, P_TESTPACKET1_ACK,
	P_TESTPACKET2_REQ, P_TESTPACKET2_ACK,
	P_TESTPACKET3_REQ, P_TESTPACKET3_ACK
};

struct Login

{
	char ID[10];
	char password[15];
};
