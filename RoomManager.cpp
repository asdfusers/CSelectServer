#include "stdafx.h"
#include "RoomManager.h"

CRoomManager* CRoomManager::m_pInst = NULL;

CRoomManager::CRoomManager() : roomIdx(1)
{
}


CRoomManager::~CRoomManager()
{
}

CRoomManager * CRoomManager::getinst()
{
	if (m_pInst == NULL)
		m_pInst = new CRoomManager;

	return m_pInst;
}

void CRoomManager::releaseInst()
{
	if (m_pInst != NULL)
		delete m_pInst;

	m_pInst = NULL;
}

void CRoomManager::insertRoom(CGameRoom& room)
{
	room.SetRoomNumber(roomIdx++);
	RoomPool.push_back(room);
}

void CRoomManager::deleteRoom(CGameRoom room)
{
	std::list<CGameRoom>::iterator itr;
	itr = RoomPool.begin();
	
	while (itr != RoomPool.end())
	{
		if (itr->GetRoomNumber() == room.GetRoomNumber())
		{
			itr = RoomPool.erase(itr++);
			break;
		}
		else
			++itr;
	}
}

void CRoomManager::updateRoom(CGameRoom room)
{

}

int CRoomManager::findRoomNumber(CGameRoom room)
{
	std::list<CGameRoom>::iterator itr;
	itr = RoomPool.begin();

	while (itr != RoomPool.end())
	{
		if (itr->GetRoomNumber() == room.GetRoomNumber())
		{
			return itr->GetRoomNumber();
		}
		else
			++itr;
	}
	return NULL;
}

CGameRoom CRoomManager::findRoom(int roomNumber)
{

	std::list<CGameRoom>::iterator itr;
	itr = RoomPool.begin();

	while (itr != RoomPool.end())
	{
		if (itr->GetRoomNumber() == roomNumber)
		{
			break;
		}
		else
			++itr;
	}
	return *itr;
}
