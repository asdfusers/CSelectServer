#include "stdafx.h"
#include "SelectThread.h"
#include "Server.h"
#include "Sockets.h"
#include "CriticalSections.h"

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
		// ���� �� �ʱ�ȭ


		FD_ZERO(&rset);
		FD_ZERO(&wset);

		FD_SET(mListen, &rset);
		// ���� �� ����
		for (int i = 0; i < CServer::getInstance()->socketNumb; i++)
		{
			if (CServer::getInstance()->g_SocketArray[i]->recvBytes > CServer::getInstance()->g_SocketArray[i]->sendBytes)
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
				// ������ �ޱ�
				retVal = recv(pInfo->sock, pInfo->buffer, MAX_BUFFER_SIZE, 0);
				if (retVal == SOCKET_ERROR)
				{
					if (WSAGetLastError() != WSAEWOULDBLOCK)
					{
						cout << "recv()" << endl;
						RemoveSocketInfo(i);
						continue;
					}
				}
				else
				{
					pInfo->recvBytes = retVal;

					//���� ������ ���
					int AddrLen = sizeof(clientAddr);
					pInfo->buffer[retVal] = '\0';
					std::cout << pInfo->buffer << std::endl;
				}
			}
			if (FD_ISSET(pInfo->sock, &wset))
			{
				// �����ͺ�����
				retVal = send(pInfo->sock, pInfo->buffer + pInfo->sendBytes,
					pInfo->recvBytes - pInfo->sendBytes, 0);

				if (retVal == SOCKET_ERROR)
				{
					if (WSAGetLastError() != WSAEWOULDBLOCK)
					{
						cout << "send()" << endl;
						RemoveSocketInfo(i);
						continue;
					}
				}

				pInfo->sendBytes += retVal;

				if (pInfo->recvBytes == pInfo->sendBytes)
					pInfo->recvBytes = pInfo->sendBytes = 0;
			}
		}
	}
}

bool CSelectThread::RemoveSocketInfo(int nIndex)
{
	CSockets *ptr = CServer::getInstance()->g_SocketArray[nIndex];

	// Ŭ���̾�Ʈ ���� ���
	SOCKADDR_IN clientaddr;
	int addrlen = sizeof(clientaddr);
	getpeername(ptr->sock, (SOCKADDR *)&clientaddr, &addrlen);
	printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
		inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

	closesocket(ptr->sock);
	delete ptr;

	for (int i = nIndex; i<CServer::getInstance()->socketNumb; i++) {
		CServer::getInstance()->g_SocketArray[i] = CServer::getInstance()->g_SocketArray[i + 1];
	}
	CServer::getInstance()->socketNumb--;
	return true;
}

bool CSelectThread::sendMessage(CSockets sock, CPacket & packet)
{
	send(sock.sock, packet.getPacketBuffer(), packet.getPacketSize(), 0);
}
