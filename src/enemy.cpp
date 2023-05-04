#include "DxLib.h"

#include "../h/enemy.h"
#include "../h/enemyMgr.h"
#include "../h/itempanel.h"
#include "../h/soundMgr.h"

elementMgr* enemy::ptr_items = nullptr;

int	enemy::GH_enemies[Enemy_MAX][ENEMY_ANIME_MAX] = { -1 };
int	enemy::GH_firedEnemies[Enemy_MAX][ENEMY_ANIME_MAX] = { -1 };

enemy::enemy() :element_canMove(true)
{
	if (GH_enemies[0][0] == -1)
	{
		for (int i = 0; i < Enemy_MAX; i++)
		{
			string path = "textures/enemy/" + to_string(i) + ".png";
			const int yNum = 2;
			int tmpGH[ENEMY_ANIME_MAX * yNum] = { -1 };
			LoadDivGraph(path.c_str(), ENEMY_ANIME_MAX * yNum, ENEMY_ANIME_MAX, yNum, 45, 45, tmpGH);

			for (int x = 0; x < ENEMY_ANIME_MAX; x++)
			{
				GH_enemies[i][x] = tmpGH[x];
				GH_firedEnemies[i][x] = tmpGH[ENEMY_ANIME_MAX + x];
			}
		}
	}
	direction = rand() % Direction_MAX;
	FrameCnt = rand() % 100;
	graphIdx = graphMAX = ENEMY_ANIME_MAX;
	faceUp = true;
}

void enemy::Update()
{
	element_canMove::Update();

	if (getExistence())
	{
		faceUp = true;

		if (fired)
		{
			if (graphIdx < ENEMY_ANIME_MAX)
			{
				if (FrameCnt % 8 == 0)
				{
					graphIdx++;
				}
			}
			else
			{
				setExistence(false);
				fired = false;

				if (ptr_items != nullptr)
				{
					int item_type = Convert_enemy_to_item(type);
					element* set_item = ptr_items->setNewElement(getHitBoxCenterIndex());

					if (item_type != itemPanel_None && set_item != nullptr)
					{
						set_item->setType(item_type);
					}
				}
			}
			faceUp = false;
		}
		else
		{
			if ((type != Enemy_Ballom && type != Enemy_Doria) || FrameCnt % 2)
			{
				vector2D<pixelPosi> newPixel = getMovePixel(getHitBoxPixel(), direction);
				move(newPixel);

				bool turn = checkHitBomb(newPixel);

				if (ptr_items != nullptr)
				{
					if (ptr_items->getExistenceBoard(createHitBoxCenterIndex(newPixel)))
					{
						turn = true;
					}
				}
				if (checkHit(newPixel) || FrameCnt % (CELL_SIZE * (8 - type)) == 0)
				{
					direction = rand() % Direction_MAX;
				}
				else if (turn)
				{
					direction = getReverseDirection(direction);
				}
			}
			if (FrameCnt % 10 == 0)
			{
				graphIdx = (++graphIdx) % graphMAX;
			}
			if (checkHitBlast(getHitBoxCenterIndex()) || checkMatchBombIndex(getHitBoxCenterIndex()))
			{
				fired = true;
				graphIdx = -1;
				soundMgr::GetInstance().Play(SE_Enemy);
				return;
			}
		}
		beltSlide();
		checkWarp(getHitBoxCenterIndex());
	}
}
void enemy::Draw()
{
	if (getExistence())
	{
		DrawEnemy(createDisplayValue().x, createDisplayValue().y - 6, type, graphIdx, fired);
	}
}
void enemy::DrawEnemy(int x, int y, int type, int animation, bool fire)
{
	if (animation >= 0 && animation < ENEMY_ANIME_MAX)
	{
		int gh = fire ? GH_firedEnemies[type][animation] : GH_enemies[type][animation];
		if (gh >= 0)
		{
			DrawGraph(x, y - 6, gh, TRUE);
		}
	}
}

void enemy::setType(int t)
{
	element_canMove::setType(t);

	moveSpeed = 1;
	graphMAX = ENEMY_ANIME_MAX;

	switch (t)
	{
	case Enemy_Ballom:
	case Enemy_ONeal:
		moveSpeed = 1;
		break;

	case Enemy_Dahl:
	case Enemy_Minvo:
		moveSpeed = 2;
		break;

	case Enemy_Doria:
		moveSpeed = 1;
		softBlockPass = true;
		break;

	case Enemy_Ovape:
		moveSpeed = 2;
		softBlockPass = true;
		break;

	case Enemy_Pass:
		moveSpeed = 3;
		break;

	case Enemy_Pontan:
		moveSpeed = 3;
		softBlockPass = true;
		graphMAX = 4;
		break;

	default:
		break;
	}
}
void enemy::setExistence(bool f)
{
	element_canMove::setExistence(f);
	graphIdx = rand() % ENEMY_ANIME_MAX;
}

int enemy::Convert_enemy_to_item(int enemyId)
{
	int repertory[10] =
	{
		itemPanel_BombKick,
		itemPanel_Punch,
		itemPanel_PowerGlove5,

		itemPanel_Heart,
		itemPanel_BombPass,
		itemPanel_SoftBlockPass,
		
		itemPanel_FireBuff,
		itemPanel_SpeedBuff,
		itemPanel_BombBuff,
		itemPanel_InvincibleSuit,
	};
	switch (enemyId)
	{
	case Enemy_Ballom:	return itemPanel_FireUp;
	case Enemy_ONeal:	return itemPanel_SpeedUp;
	case Enemy_Pass:	return itemPanel_Heart;
	default:			return repertory[rand() % 10];
	}
	return itemPanel_None;
}