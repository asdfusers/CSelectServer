#include "stdafx.h"
#include "Server.h"
#include "CriticalSectionLock.h"

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


	retVal = bind(mListen, (SOCKADDR*)&sockAddr, sizeof(sockAddr));
	if (retVal == SOCKET_ERROR)
	{
		cout << "bind()" << endl;;
	}

	retVal = listen(mListen, SOMAXCONN);
	if (retVal == SOCKET_ERROR)
	{
		cout << "listen()" << endl;;
	}

	_AcceptThread.SetSocket(mListen);
	_SelectThread.SetSocket(mListen);

	_AcceptThread.begin();
	_SelectThread.begin();
	

}

void CServer::UnInit()
{
	_AcceptThread.join();
	_SelectThread.join();
}

void CServer::CopySocketList()
{
	CCriticalSectionLock cs(_AcceptThread.cs);
	for (auto &socket : _AcceptThread.socketList)
	{
		_SelectThread.socketList.push_back(socket);
	}
	_AcceptThread.socketList.clear();

}

void CServer::Update()
{
	if (_AcceptThread.socketList.size() > 0)
		CopySocketList();

	if (!_SelectThread.recvMessageQue.messageQue.empty())
	{
		CCriticalSectionLock cs(_SelectThread.cs);
		packetParsing(_SelectThread.recvMessageQue.messageQue.front());
		_SelectThread.recvMessageQue.messageQue.pop();
	}
	if (!sendMessageQue.messageQue.empty())
	{
		sendMessage(sendMessageQue.messageQue.front());
		sendMessageQue.messageQue.pop();
	}
}

bool CServer::sendMessage(CPacket packet)
{
	retVal = send(packet.getSocketNumber(), packet.getPacketBuffer(), packet.getPacketSize(), 0);
	if (retVal == SOCKET_ERROR)
		return false;
	return true;
}


void  CServer::packetParsing(CPacket packet)
{
	switch (packet.id())
	{
	case  P_LOGINPACKET_REQ:			onLoginPacket1Req(packet);			break;
	case  P_TESTPACKET2_REQ:			onPTTestPacket2Req(packet);			break;
	case  P_TESTPACKET3_REQ:			onPTTestPacket3Req(packet);			break;
	}
}


void  CServer::onLoginPacket1Req(CPacket & packet)
{

	{
		Login log;
		packet >> log;
		std::cout << "ID : " << log.ID << "\t PASSWORD : " << log.password << std::endl;
	}

	{
		CPacket sendPacket(P_LOGINPACKET_ACK);
		sendPacket.SetSocketNumber(packet.getSocketNumber());
		sendPacket << L"Test packet 2";
		sendMessageQue.messageQue.push(sendPacket);
	}
}

void  CServer::onPTTestPacket2Req(CPacket & packet)
{
	{
		char str[127];

		packet >> (LPTSTR)str;
		printf("P_TESTPACKET2_REQ received : %s\n", str);
	}

	{
		CPacket sendPacket(P_TESTPACKET2_ACK);
		sendPacket.SetSocketNumber(packet.getSocketNumber());
		sendPacket << L"Test packet 3";
		sendMessageQue.messageQue.push(sendPacket);
	}
}

void  CServer::onPTTestPacket3Req(CPacket & packet)
{
	{
		char str[127];

		packet >> (LPTSTR)str;
		printf("P_TESTPACKET3_REQ received : %s\n", str);
	}

	{
		CPacket sendPacket(P_TESTPACKET3_ACK);
		sendPacket.SetSocketNumber(packet.getSocketNumber());
		sendPacket << L"Test packet 1";
		sendMessageQue.messageQue.push(sendPacket);

	}
}


