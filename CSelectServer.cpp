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
		if (server._AcceptThread.socketList.size() > 0)
		{
			server.CopySocketList();
		}
		for (auto user : server._SelectThread.socketList)
		{
			if (!user.recvQue.recvQue.empty())
			{
				user.recvQue.packetParsing(user.recvQue.recvQue.front(), user.sock);
				user.recvQue.recvQue.pop();
			}
			if (!user.sendQue.sendQue.empty())
			{
				user.sendQue.SendMessageW();
			}
		}
	}
		CThreadManager::getInstance()->join();

	return 0;
}

