#pragma once
#ifndef CACCEPTTHREAD_H
#define CACCEPTTHREAD_H

#include "Thread.h"
#include "Packet.h"
#include "Sockets.h"
class CAcceptThread :
	public CThread
{
public:
	CAcceptThread();
	~CAcceptThread();

public:
	virtual void threadMain();
	bool AddSocketInfo(SOCKET clientSock);
	bool sendMessage(CPacket& packet, SOCKET SOCK);
	void SetSocket(SOCKET _sock) { sock = _sock; }

	CriticalSections cs;
	std::list<CSockets> socketList;
private:
	SOCKET sock;
	SOCKADDR_IN sockAddr;
	int _addrLen;

	int retVal;
};
#endif

