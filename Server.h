#pragma once
#ifndef CSERVER_H
#define CSERVER_H

#include "stdafx.h"
#include "Sockets.h"
#include "AcceptThread.h"
#include "SelectThread.h"
#include "CriticalSections.h"
class CServer
{
public:
	CServer();
	~CServer();


public:
	void Init(int PORT);
	bool Bind();
	bool Listen();
	SOCKET getListenSocket() { return mListen; }

	static CServer* getInstance()
	{
		static CServer g_server;
		return &g_server;
	}

public:
	CSockets* g_SocketArray[FD_SETSIZE];
	int socketNumb;

	CAcceptThread _AcceptThread;
	CSelectThread _SelectThread;
	
	CriticalSections m_cs;
private:
	SOCKET mListen;
	SOCKADDR_IN sockAddr;
	int addrLen;
	int retVal;


};

#endif
