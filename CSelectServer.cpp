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
		server._SelectThread.cs.enter();
		for (auto &user : server._SelectThread.socketList)
		{

			
			if (!user.recvQue.recvQue.empty())
			{
				user.recvQue.packetParsing(user.recvQue.recvQue.front(), user.sock);
				user.recvQue.recvQue.pop();
			}

			if (!user.recvQue.sendQue.empty())
			{
				user.sendQue.sendQue.push(user.recvQue.sendQue.front());
				user.recvQue.sendQue.pop();
				user.sendQue.SendMessageW();
			}
		}
		server._SelectThread.cs.leave();

	}
		CThreadManager::getInstance()->join();

	return 0;
}

