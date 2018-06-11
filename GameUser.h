#pragma once
#include "stdafx.h"
#include "Protocol.h"
#include "Bomb.h"

class CGameUser
{
public:
	CGameUser();
	~CGameUser();


public:
	bool CreateBomb(char Maze[21][21], CGameUser* pPlayer, std::deque<CBomb>* BombQue);
	void Fire(char Maze[21][21], CGameUser* pPlayer, std::deque<CBomb> Bomb);

public:
	std::deque<CBomb> GetBombQue() { return BombQue; }
	CPosition GetPlayerPos() { return pos; }
	int GetPlayerTeam() { return iTeam; }
	int GetRoomNumber() { return iRoomNumber; }
	
	void SetPlayerTeam(int _iTeamNum) { iTeam = _iTeamNum; }
	void SetPlayerpos(int x, int y) { pos.x = x; pos.y = y; }
	void SetRoomNumber(int _RoomNumber) { iRoomNumber = _RoomNumber; }

	bool bCreateBomb;
	bool BombFire;

private:
	CPosition pos;
	int iTeam;
	int iRoomNumber;
	

	std::deque<CBomb> BombQue;

	
};

