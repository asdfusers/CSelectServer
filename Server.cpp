#include "stdafx.h"
#include "Server.h"


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
		err_quit("Socket()");

	ZeroMemory(&sockAddr, sizeof(sockaddr));
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(PORT);
	sockAddr.sin_addr.S_un.S_addr = htonl(ADDR_ANY);

	unsigned long on = true;
	retVal = ioctlsocket(mListen, FIONBIO, &on);
	if (retVal == SOCKET_ERROR)
		err_display("ioctlsocket()");
	socketNumb = 0;
	_AcceptThread.SetSocket(mListen);
	_SelectThread.SetSocket(mListen);
}

bool CServer::Bind()
{
	retVal = bind(mListen, (SOCKADDR*)&sockAddr, sizeof(sockAddr));
	if (retVal == SOCKET_ERROR)
	{
		err_quit("Bind()");
		return false;
	}
	return true;
}

bool CServer::Listen()
{
	retVal = listen(mListen, SOMAXCONN);
	if (retVal == SOCKET_ERROR)
	{
		err_quit("Listen()");
		return false;
	}
	return true;
}

void CServer::err_quit(char * msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, (LPCWSTR)msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(-1);
}

void CServer::err_display(char * msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s\n", msg, (LPCTSTR)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

