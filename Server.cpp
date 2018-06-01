#include "stdafx.h"
#include "Server.h"
#include "ThreadManager.h"

CServer::CServer()
{

}


CServer::~CServer()
{
	closesocket(mListen);
	WSACleanup();
}

void CServer::Init(int PORT)
{
	WSAData wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return;

	mListen = socket(AF_INET, SOCK_STREAM, 0);

	if (mListen == INVALID_SOCKET)
		cout << "Socket()" << endl;

	ZeroMemory(&sockAddr, sizeof(sockaddr));
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(PORT);
	sockAddr.sin_addr.S_un.S_addr = htonl(ADDR_ANY);

	unsigned long on = true;
	retVal = ioctlsocket(mListen, FIONBIO, &on);
	if (retVal == SOCKET_ERROR)
		cout << "ioctlsocket()" << endl;

	_AcceptThread.SetSocket(mListen);
	_SelectThread.SetSocket(mListen);

	_AcceptThread.begin();
	_SelectThread.begin();


}

bool CServer::Bind()
{
	retVal = bind(mListen, (SOCKADDR*)&sockAddr, sizeof(sockAddr));
	if (retVal == SOCKET_ERROR)
	{
		cout << "bind()" << endl;;
		return false;
	}
	return true;
}

bool CServer::Listen()
{
	retVal = listen(mListen, SOMAXCONN);
	if (retVal == SOCKET_ERROR)
	{
		cout << "listen()" << endl;;
		return false;
	}
	return true;
}

void CServer::CopySocketList()
{
	m_cs.enter();

	for (auto &socket : _AcceptThread.socketList)
	{
		_SelectThread.socketList.push_back(socket);
	}
	_AcceptThread.socketList.clear();
	m_cs.leave();
}

