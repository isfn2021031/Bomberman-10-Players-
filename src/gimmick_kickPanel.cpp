#include <cmath>
#include "../h/gimmick_kickPanel.h"
#include "DxLib.h"

int gimmick_kickPanel::GH_panels[Direction_MAX] = { -1 };

gimmick_kickPanel::gimmick_kickPanel()
{
	if (GH_panels[0] == -1)
	{
		LoadDivGraph("textures/gimmicks/panels.png", Direction_MAX, Direction_MAX, 1, CELL_SIZE, CELL_SIZE, GH_panels);
	}
	direction = Direction_None;
	FrameCnt = rand();
	destruction = false;
}

void gimmick_kickPanel::Update()
{
	gimmick::Update();
}
void gimmick_kickPanel::Draw()
{
	if (pBoard != nullptr)
	{
		if (getExistence())
		{
			if (direction != Direction_None)
			{
				if (pBoard->checkGroundFromIndex(getHitBoxCenterIndex()))
				{
					int drawX = BASIS_PX + getHitBoxPixel().x;
					int drawY = BASIS_PY + getHitBoxPixel().y;

					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
					DrawGraph(drawX, drawY, GH_panels[direction], TRUE);

					for (int i = 0; i < getFlashRepetitions(50); i++)
					{
						DrawGraph(drawX, drawY, GH_panels[direction], TRUE);
					}
					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
				}
			}
		}
	}
}