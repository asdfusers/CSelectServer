#pragma once
#include "stdafx.h"
#include "GameRoom.h"
class CRoomManager
{
private:
	CRoomManager();
	
	~CRoomManager();

public:
	static CRoomManager* getinst();
	static void releaseInst();

	void insertRoom(CGameRoom& room);
	void deleteRoom(CGameRoom room);
	int findRoomNumber(CGameRoom room);

	std::list<CGameRoom>::iterator findRoom(int roomNumber);
	std::list<CGameRoom> getRoomPool() { return RoomPool; }

private:
	std::list<CGameRoom> RoomPool;
	std::list<CGameRoom>::iterator itr;
	static CRoomManager* m_pInst;
	int roomIdx;
};

