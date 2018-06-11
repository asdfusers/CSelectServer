#pragma once
#include "stdafx.h"
#include "Room.h"
#include "Stage.h"
#include "GameUser.h"
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
	CStage* getStage() { return m_Stage; }
	int GetRoomNumber() { return iRoomNumber; }
	int GetReadyCnt() { return iReadyCnt; }
	void setReadyCnt() {
		iReadyCnt++;
	}
	std::map<SOCKET, CSockets&> getPool() { return PlayerPool; }

	void GameStart();

private:
	int iRoomNumber;
	int iReadyCnt = 0;
	std::map<SOCKET, CSockets&> PlayerPool;
	CStage* m_Stage;
};

