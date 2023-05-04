#include <cmath>
#include "DxLib.h"
#include "../h/gimmick_conveyor.h"

int gimmick_conveyor::GH_conveyors[Direction_MAX][CONVEYOR_GRAPH_MAX] = { -1 };

gimmick_conveyor::gimmick_conveyor()
{
	direction = Direction_None;

	if (GH_conveyors[0][0] == -1)
	{
		int tmpGH[CONVEYOR_GRAPH_MAX * Direction_MAX] = { -1 };

		LoadDivGraph("textures/gimmicks/conveyors.png", CONVEYOR_GRAPH_MAX * Direction_MAX, CONVEYOR_GRAPH_MAX, Direction_MAX, CELL_SIZE, CELL_SIZE, tmpGH);

		for (int y = 0; y < Direction_MAX; y++)
		{
			for (int x = 0; x < CONVEYOR_GRAPH_MAX; x++)
			{
				GH_conveyors[y][x] = tmpGH[y * CONVEYOR_GRAPH_MAX + x];
			}
		}
	}
}

void gimmick_conveyor::Update()
{
	gimmick::Update();

	if (FrameCnt % 6 == 0)
	{
		graphIdx = (++graphIdx) % CONVEYOR_GRAPH_MAX;
	}
}
void gimmick_conveyor::Draw()
{
	if (getExistence())
	{
		if (direction != Direction_None)
		{
			if (pBoard != nullptr)
			{
				if (pBoard->checkGroundFromIndex(getHitBoxCenterIndex()))
				{
					int drawX = BASIS_PX + getHitBoxPixel().x;
					int drawY = BASIS_PY + getHitBoxPixel().y;

					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
					DrawGraph(drawX, drawY, GH_conveyors[direction][graphIdx], TRUE);

					for (int i = 0; i < getFlashRepetitions(120); i++)
					{
						DrawGraph(drawX, drawY, GH_conveyors[direction][graphIdx], TRUE);
					}
					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
				}
			}
		}
	}
}