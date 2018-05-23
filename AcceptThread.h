#pragma once
#ifndef CACCEPTTHREAD_H
#define CACCEPTTHREAD_H

#include "Thread.h"

class CAcceptThread :
	public CThread
{
public:
	CAcceptThread();
	~CAcceptThread();

public:
	virtual void threadMain();
	bool AddSocketInfo(SOCKET clientSock);

	void SetSocket(SOCKET _sock) { sock = _sock; }
private:
	SOCKET sock;
	SOCKADDR_IN sockAddr;
	int _addrLen;
	
};
#endif

