#pragma once
#include "CriticalSections.h"
#include "Packet.h"
#include <queue>

class CSendQue
{
public:
	CSendQue();
	~CSendQue();
	bool SendMessage();
	void SetSocket(SOCKET _sock) { socket = _sock; }

public:
	SOCKET socket;
	std::queue<CPacket> sendQue;
	CriticalSections cs;
};

