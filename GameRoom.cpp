#include "stdafx.h"
#include "GameRoom.h"


CGameRoom::CGameRoom()
{
}


CGameRoom::~CGameRoom()
{
}

void CGameRoom::insertUserPool(CSockets& User)
{
	User.setRoomNum(iRoomNumber);
	PlayerPool.insert(std::pair<SOCKET, CSockets>(User.sock, User));
}

void CGameRoom::deleteUserPool(CSockets User)
{
	PlayerPool.erase(User.sock);
}

CSockets CGameRoom::findUserPool(SOCKET socket)
{
	std::map<SOCKET, CSockets>::iterator itr;
	itr = PlayerPool.begin();

	while (itr != PlayerPool.end())
	{
		if (itr->first == socket)
			return itr->second;

		else
			itr++;
	}
	return NULL;
}
