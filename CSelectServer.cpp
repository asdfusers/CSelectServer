// CSelectServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Server.h"
#include "CriticalSectionLock.h"



int main()
{
	CServer server;
	server.Init(9000);
	
	while (1)
	{	
		Sleep(1);
		server.Update();
	}
	server.UnInit();
	return 0;
}
