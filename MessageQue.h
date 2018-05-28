#pragma once
#include "stdafx.h"
#include "Packet.h"
#include "Sockets.h"

class CMessageQue
{
public:
	CMessageQue();
	~CMessageQue();

	void packetParsing(CPacket packet, CSockets socket);
	void onSend(CSockets socket);

	void	onPTTestPacket1Req(CPacket& packet, CSockets socket);
	void	onPTTestPacket2Req(CPacket& packet, CSockets socket);
	void	onPTTestPacket3Req(CPacket& packet, CSockets socket);

public:
	std::list<CPacket> messageQue;
};

