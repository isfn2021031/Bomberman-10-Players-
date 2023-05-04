#include <fstream>
#include "DxLib.h"
#include "../h/board_withGimmick.h"
#include "../h/configLoader.h"
#include "../h/element_canMove.h"
#include "../h/myfunc.h"
#include "../h/gimmick_enemyPoint.h"
#include "../h/enemy.h"

string board_withGimmick::EditFileName;

board_withGimmick::board_withGimmick()
{
	// この関数を呼び出すのはあらゆるエレメントを生成する前
	element_canMove::Initialize();

	gimmicks = new gimmickMgr(this);

	configLoader cl(FILEPATH_CONF, CONFIG_MAX);
	config_enemies = (cl.getConfig(CONFIG_Enemies) == 1);
	enemies = new enemyMgr;
	gimmick_enemyPoint::SetEnemyMgr(enemies);

	bombs = new bombMgr;

	if (bombs != nullptr)
	{
		bombs->setPlayerID(BOMB_GIMMICK);
	}
}
board_withGimmick::~board_withGimmick()
{
	if (gimmicks != nullptr)
	{
		delete gimmicks;
		gimmicks = nullptr;
	}
	if (enemies != nullptr)
	{
		delete enemies;
		enemies = nullptr;
		gimmick_enemyPoint::SetEnemyMgr(nullptr);
	}
	if (bombs != nullptr)
	{
		delete bombs;
		bombs = nullptr;
	}
}

void board_withGimmick::Update()
{
	if (gimmicks != nullptr)
	{
		gimmicks->Update();
	}
	if (enemies != nullptr)
	{
		enemies->Update();
	}
	if (bombs != nullptr)
	{
		bombs->Update();
	}
}
void board_withGimmick::Draw()
{
	board_base::Draw();
	element_canMove::DrawShadows();

	if (gimmicks != nullptr)
	{
		gimmicks->Draw();
	}
	if (bombs != nullptr)
	{
		bombs->Draw();
	}
}

void board_withGimmick::LoadBoardToFile()
{
	fstream fin(EditFileName, ios::binary | ios::in);
	if (fin)
	{
		int gimmickIDs[BOARD_HEIGHT][BOARD_WIDTH];
		
		for (int y = 0; y < BOARD_HEIGHT; y++)
		{
			for (int x = 0; x < BOARD_WIDTH; x++)
			{
				int readValue = 0;
				fin >> readValue;

				bool isCell = (readValue < IDENTIFICATION);
				cells[y][x].type = isCell ? readValue : groundId;
				
				bool isGimmick = (!isCell && (readValue < (IDENTIFICATION * 2)));
				gimmickIDs[y][x] = isGimmick ? readValue - IDENTIFICATION : GimmickID_None;
				
				bombIDs[y][x] = (!isCell && !isGimmick) ? readValue - (IDENTIFICATION * 2) : BombType_None;
			}
		}
		fin >> positionPattern;
		fin.close();

		for (int y = 0; y < BOARD_HEIGHT; y++)
		{
			for (int x = 0; x < BOARD_WIDTH; x++)
			{
				gimmicks->createNewGimmick({ x, y }, gimmickIDs[y][x]);

				if (bombIDs[y][x] != BombType_None)
				{
					bombStatus bs;
					bs.bombType = bombIDs[y][x];
					bs.power = 2 + rand() % 3;
					bombs->setNewBomb({ x, y }, bs);
				}
			}
		}
	}
}
void board_withGimmick::SaveBoardToFile()
{
	string FileName = getSaveFileName({ "stg file\0*.stg;\0","Save Stage." });
	fstream fout(FileName, ios::binary | ios::out);

	if (fout)
	{
		for (int y = 0; y < BOARD_HEIGHT; y++)
		{
			for (int x = 0; x < BOARD_WIDTH; x++)
			{
				int writeValue = cells[y][x].type;
				int gimmickID = gimmicks->getID({ x, y });

				if (bombIDs[y][x] != BombType_None)
				{
					writeValue = bombIDs[y][x] + IDENTIFICATION * 2;
				}
				if (gimmickID != GimmickID_None)
				{
					writeValue = gimmickID + IDENTIFICATION;
				}
				
				fout << writeValue << " ";
			}
		}
		fout << positionPattern << " ";
		fout.close();
	}
}

void board_withGimmick::DigStartPoint(vector2D<int> _idx, bool breakHard, int n)
{
	if (!checkOffScreen(_idx))
	{
		// 開始位置付近に最低3マス歩けるスペース作成
		cellParam ground;
		ground.type = groundId;
		ground.dug = true;
		cells[_idx.y][_idx.x] = ground;
		setCell(_idx);

		int directionX = (_idx.x < (BOARD_WIDTH / 2)) ? Direction_Right : Direction_Left;
		int directionY = (_idx.y < (BOARD_HEIGHT / 2)) ? Direction_Down : Direction_Up;

		vector<int> directions;
		directions.push_back(directionX);
		directions.push_back(directionY);
		directions.push_back(getReverseDirection(directionX));
		directions.push_back(getReverseDirection(directionY));

		int cnt = 0;

		for (int i = 0; i < directions.size(); i++)
		{
			if (cnt >= n)
			{
				break;
			}
			vector2D<int> newIdx = _idx + getDirectionSigns(directions[i]);

			if (!checkOffScreen(newIdx))
			{
				if (breakHard || checkGroundFromIndex(newIdx) || cells[newIdx.y][newIdx.x].type == CellId_SoftBlock)
				{
					cells[newIdx.y][newIdx.x] = ground;
					setCell(newIdx);
					cnt++;
				}
			}
		}
	}
}

void board_withGimmick::placePlayers()
{
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		DigStartPoint(elementMgr::StartPositions[positionPattern][i]);

		gimmick* gimmickptr = gimmicks->getGimmick(elementMgr::StartPositions[positionPattern][i]);
		if (gimmickptr != nullptr)
		{
			if (gimmickptr->getDestruction())
			{
				gimmicks->deleteGimmick(elementMgr::StartPositions[positionPattern][i]);
			}
		}
	}
}
void board_withGimmick::placeEnemies()
{
	if (config_enemies && enemies != nullptr)
	{
		for (int y = BOARD_SHIFT_Y + 1; y < BOARD_HD; y++)
		{
			for (int x = BOARD_SHIFT_X + 1; x < BOARD_WD; x++)
			{
				if (checkGroundFromIndex({ x, y }) && !cells[y][x].dug && !countWays({ x, y }))
				{
					enemies->setNewElement({ x, y });
				}
			}
		}
	}
}
void board_withGimmick::placeEnemies_sudden()
{
	if (enemies != nullptr)
	{
		for (int y = BOARD_SHIFT_Y + 1; y < BOARD_HD; y++)
		{
			for (int x = BOARD_SHIFT_X + 1; x < BOARD_WD; x++)
			{
				if (rand() % 10 == 0 && checkGroundFromIndex({ x, y }))
				{
					if (element_canMove::GetMatchShadow({ x, y }) == nullptr)
					{
						element* placed_ptr = enemies->setNewElement({ x, y });
						if (placed_ptr != nullptr)
						{
							placed_ptr->setType(Enemy_Pontan);
						}
					}
				}
			}
		}
	}
}

void board_withGimmick::setBombID(vector2D<int> _idx, int bombID)
{
	if (!checkOffScreen(_idx))
	{
		bombIDs[_idx.y][_idx.x] = bombID;
	}
}

element* board_withGimmick::getEnemy(vector2D<int> _idx)
{
	if (enemies != nullptr)
	{
		return enemies->getElementFromIndex(_idx);
	}
	return nullptr;
}