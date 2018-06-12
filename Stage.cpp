#include "stdafx.h"
#include "Stage.h"
#include "RoomManager.h"

CStage::CStage()
{
	m_Player[0].SetPlayerTeam(1);
	m_Player[1].SetPlayerTeam(2);
}

bool CStage::CheckMap(int x, int y)
{
	if (y < 0 || y >= STAGE_HEIGHT)
		return false;

	else if (x < 0 || x >= STAGE_WIDTH)
		return false;

	else if (m_Stage[y][x] == '0')
		return false;

	return true;
}

char CStage::GetStage(int x, int y)
{
	return m_Stage[x][y];
}

void CStage::SetConsolePos(int x, int y)
{
	COORD pos = { x * 2, y };
	SetConsoleCursorPosition(m_hConsole, pos);
}

bool CStage::Init()
{
	m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	m_Player[0].SetPlayerpos(0, 0);
	m_Player[1].SetPlayerpos(19, 19);


	strcpy_s(m_Stage[0], "11100000000000000000");
	strcpy_s(m_Stage[1], "00111111111100000000");
	strcpy_s(m_Stage[2], "00100010000111111100");
	strcpy_s(m_Stage[3], "01100010000000000100");
	strcpy_s(m_Stage[4], "01000011110001111100");
	strcpy_s(m_Stage[5], "01000000001111000000");
	strcpy_s(m_Stage[6], "01100000001000000000");
	strcpy_s(m_Stage[7], "00100000001111111000");
	strcpy_s(m_Stage[8], "00001110000000001000");
	strcpy_s(m_Stage[9], "01111011111111111000");
	strcpy_s(m_Stage[10], "01000000000000000000");
	strcpy_s(m_Stage[11], "01111110011111100000");
	strcpy_s(m_Stage[12], "00000011110000111110");
	strcpy_s(m_Stage[13], "01111110000000000010");
	strcpy_s(m_Stage[14], "01000000001111111110");
	strcpy_s(m_Stage[15], "01111110011000000000");
	strcpy_s(m_Stage[16], "00000010010000000000");
	strcpy_s(m_Stage[17], "01111110011111000000");
	strcpy_s(m_Stage[18], "01000000000001100000");
	strcpy_s(m_Stage[19], "11000000000000111111");

	return true;
}

void CStage::Render(char m_pStage[STAGE_HEIGHT][STAGE_WIDTH])
{
	for (int i = 0; i < 20; ++i)
	{
		for (int j = 0; j < 20; j++)
		{
			if (m_pStage[i][j] == '4')
				cout << "♨";
			else if (m_pStage[i][j] == '0')
				cout << "■";
			else if (m_pStage[i][j] == '1')
				cout << "  ";
			else if (m_pStage[i][j] == '2')
				cout << "★";
			else if (m_pStage[i][j] == '3')
				cout << "◎";
			else if (m_pStage[i][j] == '5')
				cout << "▩";
			else if (m_pStage[i][j] == '6')
				cout << "※";
			else if (m_pStage[i][j] == '7')

				cout << "◇";

		}
		cout << endl;
	}

	cout << endl;
	cout << endl;
	cout << endl;
	cout << endl;
	cout << endl;
	cout << "t : 폭탄설치 " << endl;
	cout << "w : 위, s : 아래, a : 왼쪽, d : 오른쪽, q : 종료 : ";
}

void CStage::RenderCharacter()
{
	SetConsolePos(m_Player[0].GetPlayerPos().x, m_Player[0].GetPlayerPos().y);
	std::cout << "★";
	SetConsolePos(m_Player[1].GetPlayerPos().x, m_Player[1].GetPlayerPos().y);
	std::cout << "☆";
}

bool CStage::CheckMove(char Stage[21][21], CGameUser& pPlayer, char key)
{
	switch (key)
	{
	case 'a':
	{
		if (pPlayer.GetPlayerPos().x - 1 >= 0)
		{
			//벽인지 체크한다.
			if (Stage[pPlayer.GetPlayerPos().y][pPlayer.GetPlayerPos().x - 1] != '0' &&
				Stage[pPlayer.GetPlayerPos().y][pPlayer.GetPlayerPos().x - 1] != '4')
			{
				int num = pPlayer.GetPlayerPos().x;
				--num;
				pPlayer.SetPlayerpos(num, pPlayer.GetPlayerPos().y);
				if (pPlayer.GetPlayerTeam() == 1)
				{
					CRoomManager::getinst()->findRoom(pPlayer.GetRoomNumber())->getStage()->m_Player[0].SetPlayerpos(pPlayer.GetPlayerPos().x, pPlayer.GetPlayerPos().y);
				}
				else
				{
					CRoomManager::getinst()->findRoom(pPlayer.GetRoomNumber())->getStage()->m_Player[1].SetPlayerpos(pPlayer.GetPlayerPos().x, pPlayer.GetPlayerPos().y);
				}
				return true;

			}
			else
				return false;
		}
		return false;

		break;

	}
	case 'd':
	{
		if (pPlayer.GetPlayerPos().x + 1 < 20)
		{
			//벽인지 체크한다.
			if (Stage[pPlayer.GetPlayerPos().y][pPlayer.GetPlayerPos().x+ 1] != '0' &&
				Stage[pPlayer.GetPlayerPos().y][pPlayer.GetPlayerPos().x+ 1] != '4')
			{
				int num = pPlayer.GetPlayerPos().x;
				++num;
				pPlayer.SetPlayerpos(num, pPlayer.GetPlayerPos().y);
				if (pPlayer.GetPlayerTeam() == 1)
				{
					CRoomManager::getinst()->findRoom(pPlayer.GetRoomNumber())->getStage()->m_Player[0].SetPlayerpos(pPlayer.GetPlayerPos().x, pPlayer.GetPlayerPos().y);
				}
				else
				{
					CRoomManager::getinst()->findRoom(pPlayer.GetRoomNumber())->getStage()->m_Player[1].SetPlayerpos(pPlayer.GetPlayerPos().x, pPlayer.GetPlayerPos().y);

				}
				return true;

			}

			else
				return false;

		}
		return false;
		break;
	}
	case 'w':
	{
		if (pPlayer.GetPlayerPos().y - 1 >= 0)
		{
			//벽인지 체크한다.
			if (Stage[pPlayer.GetPlayerPos().y - 1][pPlayer.GetPlayerPos().x] != '0' &&
				Stage[pPlayer.GetPlayerPos().y - 1][pPlayer.GetPlayerPos().x] != '4')
			{

				int num = pPlayer.GetPlayerPos().y;
				--num;
				pPlayer.SetPlayerpos(pPlayer.GetPlayerPos().x, num);
				if (pPlayer.GetPlayerTeam() == 1)
				{
					CRoomManager::getinst()->findRoom(pPlayer.GetRoomNumber())->getStage()->m_Player[0].SetPlayerpos(pPlayer.GetPlayerPos().x, pPlayer.GetPlayerPos().y);
				}
				else
				{
					CRoomManager::getinst()->findRoom(pPlayer.GetRoomNumber())->getStage()->m_Player[1].SetPlayerpos(pPlayer.GetPlayerPos().x, pPlayer.GetPlayerPos().y);
				}

				return true;

			}

			else
				return false;

		}
		return false;
		break;
	}
	case 's':
	{
		if (pPlayer.GetPlayerPos().y + 1 < 20)
		{
			//벽인지 체크한다.
			if (Stage[pPlayer.GetPlayerPos().y + 1][pPlayer.GetPlayerPos().x] != '0' &&
				Stage[pPlayer.GetPlayerPos().y + 1][pPlayer.GetPlayerPos().x] != '4')
			{
				int num = pPlayer.GetPlayerPos().y;
				++num;
				pPlayer.SetPlayerpos(pPlayer.GetPlayerPos().x, num);
				if (pPlayer.GetPlayerTeam() == 1)
				{
					CRoomManager::getinst()->findRoom(pPlayer.GetRoomNumber())->getStage()->m_Player[0].SetPlayerpos(pPlayer.GetPlayerPos().x, pPlayer.GetPlayerPos().y);
				}
				else
				{
					CRoomManager::getinst()->findRoom(pPlayer.GetRoomNumber())->getStage()->m_Player[1].SetPlayerpos(pPlayer.GetPlayerPos().x, pPlayer.GetPlayerPos().y);

				}
				return true;

			}

			else
				return false;
		}
		return false;
		break;
	}

	case 'r':
	{
		if (Stage[pPlayer.GetPlayerPos().y][pPlayer.GetPlayerPos().x] == '1')
		{

			if (!pPlayer.CreateBomb(Stage, &pPlayer))
				return false;

			if (pPlayer.GetPlayerTeam() == 1)
			{
				CRoomManager::getinst()->findRoom(pPlayer.GetRoomNumber())->getStage()->m_Player[0].GetBombQue() = pPlayer.GetBombQue();
				CRoomManager::getinst()->findRoom(pPlayer.GetRoomNumber())->getStage()->m_Player[0].bCreateBomb = true;
			}
			else
			{
				CRoomManager::getinst()->findRoom(pPlayer.GetRoomNumber())->getStage()->m_Player[1].GetBombQue() = pPlayer.GetBombQue();
				CRoomManager::getinst()->findRoom(pPlayer.GetRoomNumber())->getStage()->m_Player[1].bCreateBomb = true;
			}
			return true;
		}
		else
			return false;
		break;
	}

	case 't':
	case 'T':
	{
		pPlayer.Fire(Stage, &pPlayer);

		if (pPlayer.GetPlayerTeam() == 1)
		{
			CRoomManager::getinst()->findRoom(pPlayer.GetRoomNumber())->getStage()->m_Player[0].GetBombQue() = pPlayer.GetBombQue();
			CRoomManager::getinst()->findRoom(pPlayer.GetRoomNumber())->getStage()->m_Player[0].BombFire = true;
			CRoomManager::getinst()->findRoom(pPlayer.GetRoomNumber())->getStage()->m_Player[0].GetBombQue().clear();
		}
		else
		{
			CRoomManager::getinst()->findRoom(pPlayer.GetRoomNumber())->getStage()->m_Player[1].GetBombQue() = pPlayer.GetBombQue();
			CRoomManager::getinst()->findRoom(pPlayer.GetRoomNumber())->getStage()->m_Player[1].BombFire = true;
			CRoomManager::getinst()->findRoom(pPlayer.GetRoomNumber())->getStage()->m_Player[1].GetBombQue().clear();
		}
		return true;
		break;
	}


	}
	return false;
}
