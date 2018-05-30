#pragma once
#ifndef CSELECTTHREAD_H
#define CSELECTTHREAD_H
#include "Thread.h"
#include "Packet.h"
#include "RecvQue.h"
#include "SendQue.h"
#include "Sockets.h"

class CSelectThread :
	public CThread
{
public:
	CSelectThread();
	~CSelectThread();

public:
	virtual void threadMain();
	void SetSocket(SOCKET _sock) { mListen = _sock; }
	bool RemoveSocketInfo(CSockets _socket);
	int onReceive(CSockets& socket);


	std::list<CSockets> socketList;
	
private:
	FD_SET rset;
	FD_SET wset;
	SOCKET mListen;
	SOCKADDR_IN clientAddr;
	
	std::list<CSockets>::iterator itr;
	std::queue<CPacket> recvQue;
	std::queue<CPacket> sendQue;
	CriticalSections cs;
	int retVal;
};
#endif

