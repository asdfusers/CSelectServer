#include "stdafx.h"
#include "Server.h"
#include "CriticalSectionLock.h"
#include "UserManager.h"
#include "RoomManager.h"
#include "Lobby.h"
#include "GameRoom.h"
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
		CUserManager::getInst()->insertUser(socket);
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

bool CServer::broadCast(CPacket packet)
{
	for (auto socket : _SelectThread.socketList)
	{
		retVal = send(socket.sock, packet.getPacketBuffer(), packet.getPacketSize(), 0);
		if (retVal == SOCKET_ERROR)
			return false;
	}
	return true;
}


void  CServer::packetParsing(CPacket packet)
{
	switch (packet.id())
	{
	case  P_LOGINPACKET_REQ:			onLoginPacket1Req(packet);			break;
	case  P_LOBBYOPTION_REQ:			onPSelectLobbyOption(packet);			break;
	case  P_ENTERROOM_REQ:				onPPlayerEnterRoom(packet);			break;
	}
}


void  CServer::onLoginPacket1Req(CPacket & packet)
{

	{
		Login log;
		packet >> log;
		std::cout << "ID : " << log.ID << "\t PASSWORD : " << log.password << std::endl;
		CUserManager::getInst()->clientPool.find(packet.getSocketNumber())->second.setID(log.ID);
		CUserManager::getInst()->clientPool.find(packet.getSocketNumber())->second.setiLevel(1);
		CUserManager::getInst()->clientPool.find(packet.getSocketNumber())->second.setStatus(Lobby);
		CLobby::getInst()->insertUserPool(CUserManager::getInst()->clientPool.find(packet.getSocketNumber())->second);

	}

	{
		CPacket sendPacket(P_LOGINPACKET_ACK);
		sendPacket.SetSocketNumber(packet.getSocketNumber());
		sendPacket << L"여기는 로비입니다. \n1. 방 만들기\n2. 방 입장하기 ";
		sendMessageQue.messageQue.push(sendPacket);
	}
}

void  CServer::onPSelectLobbyOption(CPacket & packet)
{
		int iNumber;
		packet >> iNumber;
		printf("%d\n", iNumber);
		DeleteUserPool(packet.getSocketNumber());
		ChoiceLobbyOption(iNumber, packet.getSocketNumber());


		
		switch (iNumber)
		{
		case 1:
		{
			std::string str = "********************* 방을 개설하였습니다. *********************\n";
			CPacket sendPacket(P_LOBBYOPTION_ACK);
			sendPacket.SetSocketNumber(packet.getSocketNumber());
			sendPacket << str;
			sendMessageQue.messageQue.push(sendPacket);
		}
			break;
		case 2:
		{
			std::string str = "********************* 방 리스트 *********************\n";
			
			for (auto RoomList : CRoomManager::getinst()->getRoomPool())
			{
				str += std::to_string(RoomList.GetRoomNumber());
				str += "\n";
			}
			CPacket sendPacket(P_LOBBYOPTION_ACK);
			sendPacket.SetSocketNumber(packet.getSocketNumber());
			sendPacket << str;
			sendMessageQue.messageQue.push(sendPacket);
		}
			break;
		}

	
}

void  CServer::onPPlayerEnterRoom(CPacket & packet)
{
	{
		int iInput;
		packet >> iInput;
		printf("%d\n", iInput);

		DeleteUserPool(packet.getSocketNumber());
		CUserManager::getInst()->clientPool.find(packet.getSocketNumber())->second.setStatus(InRoom);
		CRoomManager::getinst()->findRoom(iInput).insertUserPool(CUserManager::getInst()->clientPool.find(packet.getSocketNumber())->second);
		XTrace(L"%d\n", CRoomManager::getinst()->getRoomPool().size());
		XTrace(L"%d\n", CRoomManager::getinst()->findRoom(iInput).getPool().size());
		
	}

	{
		//CPacket sendPacket(P_TESTPACKET3_ACK);
		//sendPacket.SetSocketNumber(packet.getSocketNumber());
		//sendPacket << L"Test packet 1";
		//sendMessageQue.messageQue.push(sendPacket);

	}
}

void CServer::DeleteUserPool(SOCKET socket)
{
	int num = CUserManager::getInst()->clientPool.find(socket)->second.getStatus();
	switch (num)
	{
	case 1:
		CLobby::getInst()->deleteUserPool(socket);
	break;
	case 2:

		break;

	}
}

void CServer::ChoiceLobbyOption(int iNum, SOCKET socket)
{
	switch (iNum)
	{
	case 1:
	{
		CGameRoom GameRoom;
		CRoomManager::getinst()->insertRoom(GameRoom);
		CUserManager::getInst()->clientPool.find(socket)->second.setStatus(InRoom);
		GameRoom.insertUserPool(CUserManager::getInst()->clientPool.find(socket)->second);
	
	}
		break;
	case 2:
		CUserManager::getInst()->clientPool.find(socket)->second.setStatus(InRoom);

		break;
	}
}

void CServer::ViewUserInformation(CSockets User)
{
	
	std::cout << "유저 아이디 : " << User.getID() << "\t 유저 레벨 : " << User.getLevel() << "\t 유저 상태 : " << User.getStatus() << std::endl;
}





