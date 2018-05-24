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
		for (int i = 0; i < CServer::getInstance()->socketNumb; i++)
		{
			if (CServer::getInstance()->g_SocketArray[i]->receivePacketSize > CServer::getInstance()->g_SocketArray[i]->sendPacketSize)
				FD_SET(CServer::getInstance()->g_SocketArray[i]->sock, &wset);
		
			else
				FD_SET(CServer::getInstance()->g_SocketArray[i]->sock, &rset);
		}

		//Select()
		retVal = select(0, &rset, &wset, NULL, NULL);
		if (retVal == INVALID_SOCKET)
			cout << "select()" << endl;
		
		Sleep(10);


		for (int i = 0; i < CServer::getInstance()->socketNumb; i++)
		{
			
			CSockets* pInfo = CServer::getInstance()->g_SocketArray[i];
			
			if (FD_ISSET(pInfo->sock, &rset))
			{
				onReceive(i);
				if (retVal == SOCKET_ERROR)
				{
					if (WSAGetLastError() != WSAEWOULDBLOCK)
					{
						cout << "recv()" << endl;
						RemoveSocketInfo(i);
						continue;
					}
				}
			}
			
		}
	}
}

bool CSelectThread::RemoveSocketInfo(int nIndex)
{
	CSockets *ptr = CServer::getInstance()->g_SocketArray[nIndex];

	// 클라이언트 정보 얻기
	SOCKADDR_IN clientaddr;
	int addrlen = sizeof(clientaddr);
	getpeername(ptr->sock, (SOCKADDR *)&clientaddr, &addrlen);
	printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
		inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

	closesocket(ptr->sock);
	delete ptr;

	for (int i = nIndex; i<CServer::getInstance()->socketNumb; i++) {
		CServer::getInstance()->g_SocketArray[i] = CServer::getInstance()->g_SocketArray[i + 1];
	}
	CServer::getInstance()->socketNumb--;
	return true;
}

bool CSelectThread::onReceive(int idx)
{
	CPacket receivedPacket;
	DWORD bufSize = PACKETBUFFERSIZE - CServer::getInstance()->g_SocketArray[idx]->receivePacketSize;

	retVal = recv(CServer::getInstance()->g_SocketArray[idx]->sock, &CServer::getInstance()->g_SocketArray[idx]->receivedBuffer[CServer::getInstance()->g_SocketArray[idx]->receivePacketSize], bufSize, 0);
	CServer::getInstance()->g_SocketArray[idx]->receivePacketSize = retVal;

	while (CServer::getInstance()->g_SocketArray[idx]->receivePacketSize > 0)
	{
		receivedPacket.copyToBuffer(CServer::getInstance()->g_SocketArray[idx]->receivedBuffer, CServer::getInstance()->g_SocketArray[idx]->receivePacketSize);
		if (receivedPacket.isValidPacket() == true && CServer::getInstance()->g_SocketArray[idx]->receivePacketSize >= (int)receivedPacket.getPacketSize())
		{

			packetParsing(receivedPacket, idx);
			char buffer[PACKETBUFFERSIZE];
			CServer::getInstance()->g_SocketArray[idx]->receivePacketSize -= receivedPacket.getPacketSize();
			CopyMemory(buffer, (CServer::getInstance()->g_SocketArray[idx]->receivedBuffer + receivedPacket.getPacketSize()), CServer::getInstance()->g_SocketArray[idx]->receivePacketSize);
			CopyMemory(CServer::getInstance()->g_SocketArray[idx]->receivedBuffer, buffer, CServer::getInstance()->g_SocketArray[idx]->receivePacketSize);
		}
		else
			break;
	}
	return true;
}

bool CSelectThread::sendMessage(CPacket packet, int idx)
{
	retVal = send(CServer::getInstance()->g_SocketArray[idx]->sock, packet.getPacketBuffer(), packet.getPacketSize(), 0);
	if (retVal == SOCKET_ERROR)
		return false;
	return true;
}

void CSelectThread::packetParsing(CPacket & packet, int idx)
{
	switch (packet.id())
	{	
	case  P_TESTPACKET1_REQ:			onPTTestPacket1Req(packet, idx);			break;
	case  P_TESTPACKET2_REQ:			onPTTestPacket2Req(packet, idx);			break;
	case  P_TESTPACKET3_REQ:			onPTTestPacket3Req(packet, idx);			break;
	}
}

void CSelectThread::onPTTestPacket1Req(CPacket & packet, int idx)
{

	{
		char str[127];
		packet >> (LPTSTR)str;
		printf("P_TESTPACKET1_REQ received : %s\n", str);
	}

	{
		CPacket sendPacket(P_TESTPACKET1_ACK);

		sendPacket << "Test packet 2";
		sendMessage(sendPacket, idx);
	}
}

void CSelectThread::onPTTestPacket2Req(CPacket & packet, int idx)
{
	{
		char str[127];

		packet >> (LPTSTR)str;
		printf("P_TESTPACKET2_REQ received : %s\n", str);
	}

	{
		CPacket sendPacket(P_TESTPACKET2_ACK);

		sendPacket << "Test packet 3";
		sendMessage(sendPacket, idx);
	}
}

void CSelectThread::onPTTestPacket3Req(CPacket & packet, int idx)
{
	{
		char str[127];

		packet >> (LPTSTR)str;
		printf("P_TESTPACKET3_REQ received : %s\n", str);
	}

	{
		CPacket sendPacket(P_TESTPACKET3_ACK);

		sendPacket << "Test packet 1";
		sendMessage(sendPacket, idx);
	}
}

