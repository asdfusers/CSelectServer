#include "stdafx.h"
#include "GameUser.h"


CGameUser::CGameUser()
{
	BombFire = false;
}


CGameUser::~CGameUser()
{
}

bool CGameUser::CreateBomb(char Maze[21][21], CGameUser * pPlayer, std::deque<CBomb>* BombQue)
{
	if (BombQue->size() > 4)
		return false;

	CBomb bomb;
	bomb.pos.x = pPlayer->pos.x;
	bomb.pos.y = pPlayer->pos.y;
	BombQue->push_back(bomb);
	bCreateBomb = true;
	Maze[bomb.pos.y][bomb.pos.x] = '4';

	return true;
}

void CGameUser::Fire(char Maze[21][21], CGameUser * pPlayer, std::deque<CBomb> Bomb)
{
	CPosition pos;
	pos = pPlayer->pos;

	for (auto bomb : Bomb)
	{

		Maze[bomb.pos.y][bomb.pos.x] = '1';

		for (int j = 1; j <=1; ++j)
		{
			if (bomb.pos.y - j >= 0)
			{
				if (Maze[bomb.pos.y - j][bomb.pos.x] == '0')
				{
					Maze[bomb.pos.y - j][bomb.pos.x] = '1';
				}

			}
			if (bomb.pos.y + 1 < 20)
			{
				if (Maze[bomb.pos.y + j][bomb.pos.x] == '0')
				{
					Maze[bomb.pos.y + j][bomb.pos.x] = '1';
				}
			}
			if (bomb.pos.x + j < 20)
			{
				if (Maze[bomb.pos.y][bomb.pos.x + j] == '0')
				{
					Maze[bomb.pos.y][bomb.pos.x + j] = '1';
				}
			}
			if (bomb.pos.x - j >= 0)
			{
				if (Maze[bomb.pos.y][bomb.pos.x - j] == '0')
				{
					Maze[bomb.pos.y][bomb.pos.x - j] = '1';
				}
			}
		}
	}

	Bomb.clear();
}
