#include <string>
#include "DxLib.h"

#include "../h/bombBlast.h"
#include "../h/itemMgr.h"
#include "../h/shield.h"
#include "../h/soundMgr.h"
#include "../h/RGB.h"

bool bombBlast::firstTime = true;

int bombBlast::GH_explosions[5][BLAST_ANIMATION_MAX][explosionGraph_MAX] = { -1 };

bombBlast::bombBlast()
{
	fx.cnt = 0;
	setExistence(false);

	if (firstTime)
	{
		for (int i = 0; i < 5; i++)
		{
			int tmpGH[explosionGraph_MAX * BLAST_ANIMATION_MAX] = { -1 };
			std::string FileName = "textures/explosion/" + std::to_string(i) + ".png";
			LoadDivGraph(FileName.c_str(), explosionGraph_MAX * BLAST_ANIMATION_MAX, explosionGraph_MAX, BLAST_ANIMATION_MAX, CELL_SIZE, CELL_SIZE, tmpGH);
			
			for (int y = 0; y < BLAST_ANIMATION_MAX; y++)
			{
				for (int x = 0; x < explosionGraph_MAX; x++)
				{
					GH_explosions[i][y][x] = tmpGH[explosionGraph_MAX * y + x];
				}
			}
		}
	}
	firstTime = false;
}
bombBlast::~bombBlast()
{
}

void bombBlast::Update()
{
	// ”š•—‚ÌXVˆ—
	if (getExistence())
	{
		if (fx.cnt > 0)
		{
			fx.cnt--;
		
			if (itemMgr::GetExistenceLightweight(getHitBoxCenterIndex()) != itemPanel_None
				|| shield::checkData(getHitBoxCenterIndex(), fx.direction))
			{
				dissapear(true);
			}
		}
		else
		{
			dissapear();
		}
	}
}
void bombBlast::Draw()
{
	if (getExistence())
	{
		if (fx.bombInfo.bombType == BombType_Color)
		{ 
			SetDrawBrightPlayer(fx.bombInfo.playerID);
		}
		int a = (fx.cnt / 2) - 1;
		int b = (a >= BLAST_ANIMATION_MAX) ? BLAST_ANIMATION_MAX - 1 : a;
		int c = (b < 0) ? 0 : b;

		DrawGraph(BASIS_PX + getHitBoxPixel().x, BASIS_PY + getHitBoxPixel().y, GH_explosions[fx.graphIdx][c][graphIdx], TRUE);
		SetDrawBright(255, 255, 255);
	}
}

void bombBlast::dissapear(bool hitShield)
{
	if (hitShield)
	{
		if (checkCanPenetrate(fx.bombInfo.blastType))
		{
			return;
		}
	}
	fx.cnt = 0;
	setExistence(false);
}
bool bombBlast::setForecastExplosion(blastStatus _fx)
{
	if (!checkCanPenetrate(_fx.bombInfo.blastType))
	{
		if ( shield::checkData(getHitBoxCenterIndex(), _fx.direction))
		{
			return false;
		}
	}		
	setExistence(true);

	if (_fx.cnt >= fx.cnt)
	{
		fx = _fx;
		playerID = fx.bombInfo.playerID;
		return true;
	}

	return false;
}

blastStatus bombBlast::convertBombToBlast(bombStatus bs)
{
	blastStatus fx;
	fx.cnt = 0;

	switch (bs.blastType)
	{
	case blastType_Penetrate:
		fx.graphIdx = 1;
		break;

	case blastType_SharpPenetrate:
		fx.graphIdx = 2;
		break;

	case blastType_Dynamite:
		fx.graphIdx = 4;
		break;

	case blastType_ColorP1:
	case blastType_ColorP2:
	case blastType_ColorP3:
	case blastType_ColorP4:
	case blastType_ColorP5:
	case blastType_ColorP6:
	case blastType_ColorP7:
	case blastType_ColorP8:
	case blastType_ColorP9:
	case blastType_ColorP10:
		fx.graphIdx = 3;
		break;

	default:
		fx.graphIdx = 0;
		break;
	}
	fx.direction = Direction_None;
	fx.bombInfo = bs;

	return fx;
}
