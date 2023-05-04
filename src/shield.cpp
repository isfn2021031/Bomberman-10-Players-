#include <string>
#include "DxLib.h"
#include "../h/shield.h"
#include "../h/bomb.h"
#include "../h/soundMgr.h"

int shield::GH_shield[Direction_MAX] = { -1 };

shieldData shield::StaticData[PLAYER_MAX];

shield::shield(int id)
{
	playerID = id;

	if (GH_shield[0] == -1)
	{
		for (int b = 0; b < Direction_MAX; b++)
		{
			std::string FileName = "textures/shield.png";
			LoadDivGraph(FileName.c_str(), 4, 4, 1, CELL_SIZE, CELL_SIZE, GH_shield);
		}
	}
}

void shield::Update()
{
	StaticData[playerID].existence = getExistence();
	StaticData[playerID].direction = direction;
	StaticData[playerID].idx = getHitBoxCenterIndex();
}
void shield::Draw()
{
	if (getExistence())
	{
		DrawGraph(createDisplayValue().x, createDisplayValue().y, GH_shield[direction], TRUE);
	}
}
void shield::setExistence(bool flag)
{
	if (flag > StaticData[playerID].existence)
	{
	}
	element::setExistence(flag);
}
bool shield::checkData(vector2D<int> _idx, int _direction)
{
	bool rv = false;

	_direction = getReverseDirection(_direction);

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (StaticData[i].existence)
		{ 
			// 同じマスにシールドが存在するなら
			if (_idx == StaticData[i].idx)
			{
				int dir = StaticData[i].direction;

				// 爆発中心部は何があっても防げない
				if (_direction == Direction_None)
				{
					return false;
				}
				if (_direction == dir)
				{
					rv = true;
				}
			}
		}
	}
	return rv;
}
