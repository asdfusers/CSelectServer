#pragma once
#include "stdafx.h"
#include "Room.h"

class CGameRoom :
	public CRoom
{
public:
	CGameRoom();
	~CGameRoom();

	virtual void insertUserPool(CSockets& User);
	virtual void deleteUserPool(CSockets User);
	virtual CSockets findUserPool(SOCKET socket);

	void SetRoomNumber(int index) { iRoomNumber = index; }
	
	int GetRoomNumber() { return iRoomNumber; }
	std::map<SOCKET, CSockets&> getPool() { return PlayerPool; }

private:
	int iRoomNumber;
	std::map<SOCKET, CSockets&> PlayerPool;
};

