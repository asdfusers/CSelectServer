#pragma once
#ifndef CSELECTTHREAD_H
#define CSELECTTHREAD_H

#include "Thread.h"

class CSelectThread :
	public CThread
{
public:
	CSelectThread();
	~CSelectThread();

public:
	virtual void threadMain();
	void SetSocket(SOCKET _sock) { mListen = _sock; }
	bool RemoveSocketInfo(int nIndex);
private:
	FD_SET rset;
	FD_SET wset;
	SOCKET mListen;
	SOCKADDR_IN clientAddr;
	

	int retVal;
};
#endif
