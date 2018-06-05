#pragma once
#include "Thread.h"
#include "Packet.h"
#include "Sockets.h"
#include "CriticalSections.h"

class CAcceptThread : public CThread
	
{
public:
	CAcceptThread();
	~CAcceptThread();

public:
	virtual void threadMain();
	bool AddSocketInfo(SOCKET clientSock);
	bool sendMessage(CPacket& packet, SOCKET SOCK);
	void SetSocket(SOCKET _sock) { sock = _sock; }

	CS::CriticalSection cs;
	std::list<CSockets> socketList;

private:
	SOCKET sock;
	SOCKADDR_IN sockAddr;
	int _addrLen;
	FD_SET rds;
	
	int retVal;
};
