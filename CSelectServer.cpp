// CSelectServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Server.h"
#include "ThreadManager.h"
int main()
{
	CServer::getInstance()->Init(9000);
	CServer::getInstance()->Bind();
	CServer::getInstance()->Listen();
	CServer::getInstance()->_AcceptThread.begin();
	CServer::getInstance()->_SelectThread.begin();

	CThreadManager::getInstance()->join();
	return 0;
}

