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
	switch (packet.id())
	{
	case  P_LOGINPACKET_REQ:			onLoginPacket1Req(packet, sock);			break;
	case  P_TESTPACKET2_REQ:			onPTTestPacket2Req(packet, sock);			break;
	case  P_TESTPACKET3_REQ:			onPTTestPacket3Req(packet, sock);			break;
	}
}


void CRecvQue::onLoginPacket1Req(CPacket & packet, SOCKET sock)
{

	{
		Login log;
		packet >> log;
		std::cout << "ID : " << log.ID << "\t PASSWORD : " << log.password << std::endl;
	}

	{
		CPacket sendPacket(P_LOGINPACKET_ACK);

		sendPacket << L"Test packet 2";
		cs.enter();
		sendQue.push(sendPacket);
		cs.leave();
	}
}

void CRecvQue::onPTTestPacket2Req(CPacket & packet, SOCKET sock)
{
	{
		char str[127];

		packet >> (LPTSTR)str;
		printf("P_TESTPACKET2_REQ received : %s\n", str);
	}

	{
		CPacket sendPacket(P_TESTPACKET2_ACK);

		sendPacket << L"Test packet 3";
		cs.enter();
		sendQue.push(sendPacket);
		cs.leave();
	}
}

void CRecvQue::onPTTestPacket3Req(CPacket & packet, SOCKET sock)
{
	{
		char str[127];

		packet >> (LPTSTR)str;
		printf("P_TESTPACKET3_REQ received : %s\n", str);
	}

	{
		CPacket sendPacket(P_TESTPACKET3_ACK);

		sendPacket << L"Test packet 1";
		cs.enter();
		sendQue.push(sendPacket);
		cs.leave();
		
	}
}


