#include "stdafx.h"
#include "AcceptThread.h"
#include "Server.h"
#include "Sockets.h"
#include "CriticalSections.h"
#include <iostream>

CAcceptThread::CAcceptThread()
{
}

CAcceptThread::~CAcceptThread()
{
}

void CAcceptThread::threadMain()
{
	SOCKET ConnectionSocket;
	_addrLen = sizeof(sockAddr);
	
	
	while(1)
	{
		
		ConnectionSocket = accept(sock, (SOCKADDR*)&sockAddr, &_addrLen);
		
		if (ConnectionSocket == INVALID_SOCKET)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
				cout << "accept()" << endl;
		}
		else
		{
			std::cout << "[����] Ŭ���̾�Ʈ ���� : IP[ " << inet_ntoa(sockAddr.sin_addr) << " ], \t ��Ʈ��ȣ[ " << ntohs(sockAddr.sin_port) << " ]"
				<< std::endl;
			AddSocketInfo(ConnectionSocket);
		}

	}
	
}

bool CAcceptThread::AddSocketInfo(SOCKET clientSock)
{

	if (CServer::getInstance()->socketNumb >= FD_SETSIZE - 1)
	{
		std::cout << "[����] ���� ������ �߰� �� �� �����ϴ�." << std::endl;
		return false;
	}

	CSockets* pSocket = new CSockets;

	if (pSocket == NULL)
	{
		std::cout << "[����] �޸� ���� \n" << std::endl;
		return false;

	}
	
	pSocket->sock = clientSock;
	pSocket->recvBytes = 0;
	pSocket->sendBytes = 0;



	CServer::getInstance()->g_SocketArray[CServer::getInstance()->socketNumb] = pSocket;
	CServer::getInstance()->socketNumb++;

	
	return true;
}
