#pragma once
#include "stdafx.h"
#include "GameUser.h"
#define STAGE_HEIGHT 21
#define STAGE_WIDTH 21

class CStage
{
public:
	CStage();
public:
	char m_Stage[STAGE_HEIGHT][STAGE_WIDTH];

public:
	bool CheckMap(int x, int y);
	char GetStage(int x, int y);
	void SetConsolePos(int x, int y);

	bool Init();
	void Render(char m_pStage[STAGE_HEIGHT][STAGE_WIDTH]);
	void RenderCharacter();
	bool CheckMove(char Stage[21][21], CGameUser& pPlayer, char key);

public:
	HANDLE m_hConsole;
	CGameUser m_Player[2];


};

