// CSelectServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Server.h"

int main()
{
	CServer server;
	server.Init(9000);
	server.Bind();
	server.Listen();

	server._AcceptThread.begin();
	server._SelectThread.begin();
//	CThreadManager::getInstance()->join();
	while (1)
	{
		
		server.CopySocketList();
		server.CopyMessageQue();
	}



	return 0;
}

