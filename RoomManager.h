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
	void updateRoom(CGameRoom room);
	int findRoomNumber(CGameRoom room);
	CGameRoom findRoom(int roomNumber);
	std::list<CGameRoom> getRoomPool() { return RoomPool; }

private:
	std::list<CGameRoom> RoomPool;
	static CRoomManager* m_pInst;
	int roomIdx;
};

