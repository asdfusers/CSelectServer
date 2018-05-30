#include "stdafx.h"
#include "SendQue.h"


CSendQue::CSendQue()
{
}


CSendQue::~CSendQue()
{
}

bool CSendQue::SendMessage()
{
	while (!sendQue.empty())
	{
		int retVal = send(socket, sendQue.front().getPacketBuffer(), sendQue.front().getPacketSize(), 0);
		if (retVal == SOCKET_ERROR)
			return false;
		sendQue.pop();
	}
	return true;
}
