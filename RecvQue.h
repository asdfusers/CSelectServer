#pragma once

#include "CriticalSections.h"
#include "Packet.h"
#include <queue>

class CRecvQue
{
public:
	CRecvQue();
	~CRecvQue();

public:
	void packetParsing(CPacket packet, SOCKET sock);
	void	onPTTestPacket1Req(CPacket& packet, SOCKET sock);
	void	onPTTestPacket2Req(CPacket& packet, SOCKET sock);
	void	onPTTestPacket3Req(CPacket& packet, SOCKET sock);
	void SetSocket(SOCKET _sock) { socket = _sock; }

public:
	SOCKET socket;
	std::queue<CPacket> recvQue;
	CriticalSections cs;
};

