// CSelectServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Server.h"
#include "ThreadManager.h"
int main()
{
	CServer server;
	server.Init(9000);
	server.Bind();
	server.Listen();
	

	while (1)
	{
		server.CopySocketList();
		server.CopyMessageQue();
	}
		CThreadManager::getInstance()->join();

	return 0;
}

