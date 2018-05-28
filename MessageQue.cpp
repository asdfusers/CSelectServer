#include "stdafx.h"
#include "MessageQue.h"


CMessageQue::CMessageQue()
{
}


CMessageQue::~CMessageQue()
{
}

void CMessageQue::packetParsing(CPacket packet,  CSockets socket)
{
	switch (packet.id())
	{
	case  P_TESTPACKET1_REQ:			onPTTestPacket1Req(packet, socket);			break;
	case  P_TESTPACKET2_REQ:			onPTTestPacket2Req(packet, socket);			break;
	case  P_TESTPACKET3_REQ:			onPTTestPacket3Req(packet, socket);			break;
	}
}

void CMessageQue::onSend(CSockets socket)
{
}

void CMessageQue::onPTTestPacket1Req(CPacket & packet, CSockets socket)
{

	{
		wchar_t  str[127];
		packet >> str;
		printf("P_TESTPACKET1_REQ received : %s\n", str);
	}

	{
		CPacket sendPacket(P_TESTPACKET1_ACK);
		sendPacket << L"Test packet 2";
		socket.sendQue.push_back(sendPacket);
		//		sendMessage(sendPacket, idx);
	}
}

void CMessageQue::onPTTestPacket2Req(CPacket & packet, CSockets socket)
{
	{
		wchar_t str[127];

		packet >> str;
		printf("P_TESTPACKET2_REQ received : %s\n", str);
	}

	{
		CPacket sendPacket(P_TESTPACKET2_ACK);

		sendPacket << L"Test packet 3";
		socket.sendQue.push_back(sendPacket);
		//sendMessage(sendPacket, idx);
	}
}

void CMessageQue::onPTTestPacket3Req(CPacket & packet, CSockets socket)
{
	{
		wchar_t str[127];

		packet >> str;
		printf("P_TESTPACKET3_REQ received : %s\n", str);
	}

	{
		CPacket sendPacket(P_TESTPACKET3_ACK);

		sendPacket << L"Test packet 1";
		socket.sendQue.push_back(sendPacket);
		//	sendMessage(sendPacket, idx);
	}
}

