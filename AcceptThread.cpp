#include "stdafx.h"
#include "AcceptThread.h"
#include "Server.h"
#include "Sockets.h"
#include "CriticalSections.h"
#include "Protocol.h"
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
			CPacket sendPacket(P_CONNECTIONSUCCESS_ACK);
			sendPacket << L"Welcome To Network GameLobby \nPlease Input Your ID and Password\n";
			sendMessage(sendPacket, ConnectionSocket);
			
		}

	}
	
}

bool CAcceptThread::AddSocketInfo(SOCKET clientSock)
{
	CSockets pSocket;

	pSocket.sock = clientSock;
	pSocket.receivePacketSize = 0;
	pSocket.sendPacketSize = 0;
	pSocket.Init();
	CriticalSections::getInstance()->enter();
	socketList.push_back(pSocket);
	CriticalSections::getInstance()->leave();

	return true;
}

bool CAcceptThread::sendMessage(CPacket & packet, SOCKET _SOCK)
{
	 retVal = send(_SOCK, packet.getPacketBuffer(), packet.getPacketSize(),0);
	 if (retVal == SOCKET_ERROR)
		 return false;
	 return true;
}
