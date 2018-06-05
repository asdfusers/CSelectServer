#pragma once
#include "stdafx.h"
#include "Packet.h"
class CMessageQue
{
public:
	CMessageQue();
	~CMessageQue();

public:
	std::queue<CPacket> messageQue;
};

