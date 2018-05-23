// CSelectServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Server.h"

int main()
{
	CServer::getInstance()->Init(9000);
	CServer::getInstance()->Bind();
	CServer::getInstance()->Listen();
	CServer::getInstance()->_AcceptThread.run();
	CServer::getInstance()->_SelectThread.run();

	while (1)
	{
	
	}
	return 0;
}

