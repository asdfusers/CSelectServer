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
			std::cout << "[서버] 클라이언트 접속 : IP[ " << inet_ntoa(sockAddr.sin_addr) << " ], \t 포트번호[ " << ntohs(sockAddr.sin_port) << " ]"
				<< std::endl;
			AddSocketInfo(ConnectionSocket);
		}

	}
	
}

bool CAcceptThread::AddSocketInfo(SOCKET clientSock)
{

	if (CServer::getInstance()->socketNumb >= FD_SETSIZE - 1)
	{
		std::cout << "[오류] 소켓 정보를 추가 할 수 없습니다." << std::endl;
		return false;
	}

	CSockets* pSocket = new CSockets;

	if (pSocket == NULL)
	{
		std::cout << "[오류] 메모리 부족 \n" << std::endl;
		return false;

	}
	
	pSocket->sock = clientSock;
	pSocket->recvBytes = 0;
	pSocket->sendBytes = 0;



	CServer::getInstance()->g_SocketArray[CServer::getInstance()->socketNumb] = pSocket;
	CServer::getInstance()->socketNumb++;

	
	return true;
}
