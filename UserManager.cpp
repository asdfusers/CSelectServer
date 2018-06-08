#include "stdafx.h"
#include "UserManager.h"


CUserManager* CUserManager::m_pInst = NULL;

CUserManager * CUserManager::getInst()
{
	if (m_pInst == NULL)
		m_pInst = new CUserManager();
	
	return m_pInst;
}

void CUserManager::releaseInst()
{
	if(m_pInst != NULL)
		delete m_pInst;

	m_pInst = 0;
}

void CUserManager::insertUser(CSockets& client)
{
	clientPool.insert(std::pair<SOCKET, CSockets>(client.sock, client));
}

void CUserManager::deleteUser(CSockets client)
{
	clientPool.erase(client.sock);
}

std::map<SOCKET, CSockets>::iterator CUserManager::findUser(SOCKET socket)
{
	itr = clientPool.begin();
	while (itr != clientPool.end())
	{
		if (itr->first == socket)
			break;
		else
			itr++;
	}
	return itr;
}



