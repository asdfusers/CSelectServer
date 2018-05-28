#include "stdafx.h"
#include "SelectThread.h"
#include "Server.h"
#include "Sockets.h"
#include "CriticalSections.h"
#include "Protocol.h"
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
		retVal = select(0, &rset, &wset, NULL, NULL);
		if (retVal == INVALID_SOCKET)
			cout << "select()" << endl;
		
		Sleep(5);
		for (auto socket : socketList)
		{
			if(FD_ISSET(socket.sock, &rset))
			{ 
				onReceive(socket);
			}
			else if (FD_ISSET(socket.sock, &wset))
			{

			}
		}
		


	
	}
}

bool CSelectThread::RemoveSocketInfo(CSockets _socket)
{
//	socketList.remove(_socket);
	closesocket(_socket.sock);
	return true;
}

bool CSelectThread::onReceive(CSockets socket)
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
			socket.recvQue.push_back(receivedPacket);
			socket.receivePacketSize -= receivedPacket.getPacketSize();
			CopyMemory(buffer, (socket.receivedBuffer + receivedPacket.getPacketSize()), socket.receivePacketSize);
			CopyMemory(socket.receivedBuffer, buffer, socket.receivePacketSize);
		}
		else
			break;
	}
	return true;
}
//bool CSelectThread::sendMessage(CPacket packet, int idx)
//{
//	retVal = send(CServer::getInstance()->g_SocketArray[idx]->sock, packet.getPacketBuffer(), packet.getPacketSize(), 0);
//	if (retVal == SOCKET_ERROR)
//		return false;
//	return true;
//}
