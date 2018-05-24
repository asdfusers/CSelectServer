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
		// 소켓 셋 초기화


		FD_ZERO(&rset);
		FD_ZERO(&wset);

		FD_SET(mListen, &rset);
		// 소켓 셋 지정
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
				// 데이터 받기
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

					//받은 데이터 출력
					int AddrLen = sizeof(clientAddr);
					pInfo->buffer[retVal] = '\0';
					std::cout << pInfo->buffer << std::endl;
				}
			}
			if (FD_ISSET(pInfo->sock, &wset))
			{
				// 데이터보내기
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

bool CSelectThread::sendMessage(CSockets sock, CPacket & packet)
{
	send(sock.sock, packet.getPacketBuffer(), packet.getPacketSize(), 0);
}
