#include "DxLib.h"
#include "../h/enemy_Boss.h"

int	enemy_Boss::GH_Bosses[Boss_MAX][ENEMY_ANIME_MAX] = { -1 };
int	enemy_Boss::GH_firedBosses[Boss_MAX][ENEMY_ANIME_MAX] = { -1 };

enemy_Boss::enemy_Boss()
{
	if (GH_Bosses[0][0] == -1)
	{
		for (int i = 0; i < Boss_MAX; i++)
		{
			string path = "textures/enemy_Boss/" + to_string(i) + ".png";
			const int yNum = 2;
			int tmpGH[ENEMY_ANIME_MAX * yNum] = { -1 };
			LoadDivGraph(path.c_str(), ENEMY_ANIME_MAX * yNum, ENEMY_ANIME_MAX, yNum, 45, 45, tmpGH);

			for (int x = 0; x < ENEMY_ANIME_MAX; x++)
			{
				GH_Bosses[i][x] = tmpGH[x];
				GH_firedBosses[i][x] = tmpGH[ENEMY_ANIME_MAX + x];
			}
		}
	}
	direction = rand() % Direction_MAX;
	FrameCnt = rand() % 100;
	graphIdx = graphMAX = ENEMY_ANIME_MAX;
	faceUp = true;
}

void enemy_Boss::Update()
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
			}
			faceUp = false;
		}
		else
		{
			if (FrameCnt % 2)
			{
				vector2D<pixelPosi> newPixel = getMovePixel(getHitBoxPixel(), direction);
				move(newPixel);

				if (checkHit(newPixel))
				{
					direction = rand() % Direction_MAX;
				}
				else if (checkHitBomb(newPixel))
				{
					direction = getReverseDirection(direction);
				}
			}
			if (FrameCnt % 10 == 0)
			{
				graphIdx = (++graphIdx) % graphMAX;
			}
			if (checkHitBlast() || checkMatchBombIndex(getHitBoxCenterIndex()))
			{
				fired = true;
				graphIdx = -1;
				//soundMgr::GetInstance().Play(SE_Enemy);
				return;
			}
		}
		beltSlide();
	}
}

void enemy_Boss::Draw()
{
	if (getExistence())
	{
		DrawBoss(createDisplayValue().x, createDisplayValue().y - 6, type, graphIdx, fired);
	}
}
void enemy_Boss::DrawBoss(int x, int y, int type, int animation, bool fire)
{
	if (animation >= 0 && animation < ENEMY_ANIME_MAX)
	{
		int gh = fire ? GH_firedBosses[type][animation] : GH_Bosses[type][animation];
		if (gh >= 0)
		{
			DrawGraph(x, y - 6, gh, TRUE);
		}
	}
}

void enemy_Boss::setType(int t)
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
void enemy_Boss::setExistence(bool f)
{
	element_canMove::setExistence(f);
	graphIdx = rand() % ENEMY_ANIME_MAX;
}

bool enemy_Boss::checkHitBlast()
{
	vector2D<int> idx =
	{
		getHitBoxCenterIndex().x - Size.x / 2,
		getHitBoxCenterIndex().y - Size.y / 2
	};

	for (int y = 0; y < Size.y; y++)
	{
		for (int x = 0; x < Size.x; x++)
		{
			vector2D<int> newIdx = { idx.x + x, idx.y + y };
			if (element_canMove::checkHitBlast(newIdx))
			{
				return true;
			}
		}
	}
	return false;
}