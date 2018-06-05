#pragma once
#include "Thread.h"
#include "Packet.h"
#include "Sockets.h"
#include "MessageQue.h"
#include "CriticalSections.h"
class CSelectThread : public CThread
{
public:
	CSelectThread();
	~CSelectThread();

public:
	virtual void threadMain();
	void SetSocket(SOCKET _sock) { mListen = _sock; }
	std::list<CSockets>::iterator RemoveSocketInfo(CSockets _socket);
	int onReceive(CSockets& socket);
	std::list<CSockets> socketList;
	
	CMessageQue recvMessageQue;
	CS::CriticalSection cs;

private:
	FD_SET rset;
	FD_SET wset;
	SOCKET mListen;
	SOCKADDR_IN clientAddr;
	std::list<CSockets>::iterator itr;


	int retVal;
};
