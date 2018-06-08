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
	std::cout << packet.getSocketNumber() << std::endl;
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

bool CServer::broadCastInRoom(CPacket packet)
{
	for (auto socket : _SelectThread.socketList)
	{
		if (socket.sock == packet.getSocketNumber())
			continue;
		else
		{
			retVal = send(socket.sock, packet.getPacketBuffer(), packet.getPacketSize(), 0);
			if (retVal == SOCKET_ERROR)
				return false;
		}
	}
	return true;
}


void  CServer::packetParsing(CPacket packet)
{
	switch (packet.id())
	{
	case  P_LOGINPACKET_REQ:				onLoginPacket1Req(packet);			break;
	case  P_LOBBYOPTION_REQ:				onPSelectLobbyOption(packet);		break;
	case  P_ENTERROOM_REQ:					onPPlayerEnterRoom(packet);			break;
	case  P_BROADCAST_ENTER_ROOM_REQ:		onPBroadCastEnterRoom(packet);		break;
	}
}


void  CServer::onLoginPacket1Req(CPacket & packet)
{

	{
		Login log;
		packet >> log;
		std::cout << "ID : " << log.ID << "\t PASSWORD : " << log.password << std::endl;
		CUserManager::getInst()->findUser(packet.getSocketNumber())->second.setID(log.ID);
		CUserManager::getInst()->findUser(packet.getSocketNumber())->second.setiLevel(1);
		CUserManager::getInst()->findUser(packet.getSocketNumber())->second.setStatus(Lobby);
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
			std::string str = ViewUserInformation(CUserManager::getInst()->findUser(packet.getSocketNumber())->second);
			CPacket sendPacket(P_LOBBYOPTION_ACK);
			sendPacket.SetSocketNumber(packet.getSocketNumber());
			sendPacket << str;
			sendMessageQue.messageQue.push(sendPacket);
		}
			break;
		case 2:
		{
			std::string str = "********************* 방 리스트 *********************\n";
			
			for (auto &RoomList : CRoomManager::getinst()->getRoomPool())
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
		CUserManager::getInst()->findUser(packet.getSocketNumber())->second.setStatus(InRoom);
		CUserManager::getInst()->findUser(packet.getSocketNumber())->second.setRoomNum(iInput);
		CRoomManager::getinst()->findRoom(iInput)->insertUserPool(CUserManager::getInst()->findUser(packet.getSocketNumber())->second);
	
	}

	{
		std::string str = ViewUserInformation(CUserManager::getInst()->findUser(packet.getSocketNumber())->second);

		CPacket sendPacket(P_ENTERROOM_ACK);
		sendPacket.SetSocketNumber(packet.getSocketNumber());
		sendPacket << str;
		sendMessageQue.messageQue.push(sendPacket);

	}
}

void CServer::onPBroadCastEnterRoom(CPacket & packet)
{
	{
		std::string str = ViewUserInformation(CUserManager::getInst()->findUser(packet.getSocketNumber())->second);
		std::cout << packet.getSocketNumber() << std::endl;
		for (auto &player : CRoomManager::getinst()->findRoom(CUserManager::getInst()->findUser(packet.getSocketNumber())->second.getRoomidx())->getPool())
		{
			if (player.second.sock == packet.getSocketNumber())
				continue;

			CPacket sendPacket(P_BROADCAST_ENTER_ROOM_ACK);
			sendPacket.SetSocketNumber(player.first);
			std::cout << packet.getSocketNumber() << std::endl;
			sendPacket << str;
			sendMessageQue.messageQue.push(sendPacket);
		}
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
		CRoomManager::getinst()->findRoom(GameRoom.GetRoomNumber())->insertUserPool(CUserManager::getInst()->findUser(socket)->second);
		std::map<SOCKET, CSockets>::iterator itr = CUserManager::getInst()->clientPool.begin();
		itr->second.setStatus(InRoom);
	}
		break;
	case 2:
	{
		std::map<SOCKET, CSockets>::iterator itr = CUserManager::getInst()->clientPool.begin();
		itr->second.setStatus(InRoom);
	}
		break;
	}
}

std::string CServer::ViewUserInformation(CSockets User)
{
	std::string temp;

	for (auto player : CRoomManager::getinst()->findRoom(User.getRoomidx())->getPool())
	{
		temp += "유저 아이디 : ";
		temp += player.second.getID();
		temp += "\t";
		temp += "유저 레벨 : ";
		temp += std::to_string(player.second.getLevel());
		temp += "\t";
		temp += "유저 상태 : ";
		temp += std::to_string(player.second.getStatus());
		temp += "\t";
		temp += "\n";
	}
	return temp;
}





