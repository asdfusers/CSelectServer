#include "stdafx.h"
#include "RecvQue.h"


CRecvQue::CRecvQue()
{
}


CRecvQue::~CRecvQue()
{
}

void CRecvQue::packetParsing(CPacket packet, SOCKET sock)
{
	cs.enter();
	switch (packet.id())
	{
	case  P_TESTPACKET1_REQ:			onPTTestPacket1Req(packet, sock);			break;
	case  P_TESTPACKET2_REQ:			onPTTestPacket2Req(packet, sock);			break;
	case  P_TESTPACKET3_REQ:			onPTTestPacket3Req(packet, sock);			break;
	}
	cs.leave();
}


void CRecvQue::onPTTestPacket1Req(CPacket & packet, SOCKET sock)
{
	cs.enter();
	{
		char str[127];
		packet >> (LPTSTR)str;
		printf("P_TESTPACKET1_REQ received : %s\n", str);
	}

	{
	
		CPacket sendPacket(P_TESTPACKET1_ACK);
		sendPacket << L"Test packet 2";
		sendQue.push(sendPacket);
		
	}
	cs.leave();
}

void CRecvQue::onPTTestPacket2Req(CPacket & packet, SOCKET sock)
{
	cs.enter();
	{
		char str[127];

		packet >> (LPTSTR)str;
		printf("P_TESTPACKET2_REQ received : %s\n", str);
	}

	{
	
		CPacket sendPacket(P_TESTPACKET2_ACK);

		sendPacket << L"Test packet 3";
		sendQue.push(sendPacket);
	}
	cs.leave();

}

void CRecvQue::onPTTestPacket3Req(CPacket & packet, SOCKET sock)
{
	cs.enter();

	{
		char str[127];

		packet >> (LPTSTR)str;
		printf("P_TESTPACKET3_REQ received : %s\n", str);
	}

	{
		CPacket sendPacket(P_TESTPACKET3_ACK);

		sendPacket << L"Test packet 1";
		sendQue.push(sendPacket);		
	}
	cs.leave();

}


