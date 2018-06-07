#include "stdafx.h"
#include "Lobby.h"

CLobby* CLobby::m_pInst = NULL;

void CLobby::releaseInst()
{
	if (m_pInst != NULL)
	{
		delete m_pInst;
		m_pInst = NULL;
	}
}


void CLobby::insertUserPool(CSockets& User)
{
	LobbyClientPool.insert(std::pair<SOCKET, CSockets>(User.sock, User));
}

void CLobby::deleteUserPool(CSockets User)
{
	LobbyClientPool.erase(User.sock);
}

CSockets CLobby::findUserPool(SOCKET socket)
{
	std::map<SOCKET, CSockets>::iterator itr;
	itr = LobbyClientPool.begin();

	while (itr != LobbyClientPool.end())
	{
		if (itr->first == socket)
			return itr->second;

		else
			itr++;
	}
	return NULL;
}

CLobby::CLobby()
{
}


CLobby::~CLobby()
{
}
