#include "../h/reverseSwitch.h"
#include "../h/board_main.h"

int gimmick_switch::GH_switch[2] = { -1 };

gimmick_switch::gimmick_switch()
{
	if (GH_switch[0] == -1)
	{
		LoadDivGraph("textures/gimmicks/switch.png", 2, 2, 1, CELL_SIZE, CELL_SIZE, GH_switch);
	}
	destruction = false;
}

void gimmick_switch::Update()
{
	if (getExistence())
	{
		gimmick::Update();

		if (hitPl > hitPl_old)
		{
			*pReverse = !(*pReverse);
		}
		graphIdx = *pReverse;
	}
}
void gimmick_switch::Draw()
{
	if (getExistence())
	{
		if (pBoard != nullptr)
		{
			if (pBoard->checkGroundFromIndex(getHitBoxCenterIndex()))
			{
				int drawX = BASIS_PX + getHitBoxPixel().x;
				int drawY = BASIS_PY + getHitBoxPixel().y;

				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
				DrawGraph(drawX, drawY, GH_switch[graphIdx], TRUE);

				for (int i = 0; i < getFlashRepetitions(50); i++)
				{
					DrawGraph(drawX, drawY, GH_switch[graphIdx], TRUE);
				}
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}
		}
	}
}