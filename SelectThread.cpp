#include "stdafx.h"
#include "SelectThread.h"
#include "Server.h"
#include "CriticalSectionLock.h"
#include "Protocol.h"
#include "UserManager.h"
#include "RoomManager.h"
#include <algorithm>
CSelectThread::CSelectThread()
{
}


CSelectThread::~CSelectThread()
{
}

void CSelectThread::threadMain()
{
	while (1)
	{
		// 소켓 셋 초기화
		struct timeval tv = { 0, 10 };
		FD_ZERO(&rset);
		FD_ZERO(&wset);

		FD_SET(mListen, &rset);
		// 소켓 셋 지정
		for (auto &socket : socketList)
		{
			if (socket.receivePacketSize > socket.sendPacketSize)
				FD_SET(socket.sock, &wset);
			else
				FD_SET(socket.sock, &rset);
		}

		//Select()
		retVal = select(0, &rset, &wset, NULL, &tv);
		if (retVal == INVALID_SOCKET)
			cout << "select()" << endl;

		itr = socketList.begin();
	
		for (;itr != socketList.end();)
		{			
			if (FD_ISSET(itr->sock, &rset))
			{
				retVal = onReceive(*itr);
				if (retVal == SOCKET_ERROR)
				{
					if (WSAGetLastError() != WSAEWOULDBLOCK)
					{
						CCriticalSectionLock cs(cs);
						CUserManager::getInst()->deleteUser(*itr);
						itr = RemoveSocketInfo(*itr);

						continue;
					}
				}

				else if (retVal == 0)
				{
					CCriticalSectionLock cs(cs);
					CUserManager::getInst()->deleteUser(*itr);
					itr = RemoveSocketInfo(*itr);
					
					continue;
				}

			}
			*itr++;
		}
	}
}


std::list<CSockets>::iterator CSelectThread::RemoveSocketInfo(CSockets _socket)
{
	std::list<CSockets>::iterator itr = socketList.begin();
	while(itr != socketList.end())
	{
		if (itr->sock == _socket.sock)
		{
			itr = socketList.erase(itr++);
			break;
		}
		else
			++itr;
	}
	SOCKADDR_IN clientaddr;
	int addrlen = sizeof(clientaddr);
	getpeername(_socket.sock, (SOCKADDR *)&clientaddr, &addrlen);
	printf("[서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
		inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

	closesocket(_socket.sock);
	_socket.sock = INVALID_SOCKET;
	return itr;
}

int CSelectThread::onReceive(CSockets& socket)
{
	CPacket receivedPacket;

	DWORD bufSize = PACKETBUFFERSIZE - socket.receivePacketSize;

	retVal = recv(socket.sock, &socket.receivedBuffer[socket.receivePacketSize], bufSize, 0);
	socket.receivePacketSize = retVal;

	while (socket.receivePacketSize > 0)
	{
		receivedPacket.copyToBuffer(socket.receivedBuffer, socket.receivePacketSize);

		if (receivedPacket.isValidPacket() == true && socket.receivePacketSize >= (int)receivedPacket.getPacketSize())
		{
			char buffer[PACKETBUFFERSIZE];
			CCriticalSectionLock cs(cs);
			receivedPacket.SetSocketNumber(socket.sock);
			recvMessageQue.messageQue.push(receivedPacket);
			socket.receivePacketSize -= receivedPacket.getPacketSize();
			CopyMemory(buffer, (socket.receivedBuffer + receivedPacket.getPacketSize()), socket.receivePacketSize);
			CopyMemory(socket.receivedBuffer, buffer, socket.receivePacketSize);
		}
		else
			break;
	}
	return retVal;
}

