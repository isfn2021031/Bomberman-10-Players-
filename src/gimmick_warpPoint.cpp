#include "DxLib.h"

#include "../h/gimmick_warpPoint.h"
#include "../h/soundMgr.h"
#include "../h/board_main.h"
#include "../h/global.h"

vector<gimmick_warpPoint*> gimmick_warpPoint::pInstances;

int gimmick_warpPoint::GH_warpPoint = -1;
int gimmick_warpPoint::GH_effect = -1;

gimmick_warpPoint::gimmick_warpPoint()
{
	destruction = false;
	pInstances.push_back(this);

	if (GH_warpPoint == -1)
	{
		GH_warpPoint = LoadGraph("textures/gimmicks/warpPoint.png");
	}
	if (GH_effect == -1)
	{
		GH_effect = LoadGraph("textures/gimmicks/warpEffect.png");
	}
}
gimmick_warpPoint::~gimmick_warpPoint()
{
}

void gimmick_warpPoint::Initialize()
{
	pInstances.clear();
}
void gimmick_warpPoint::Update()
{
	if (getExistence())
	{
		gimmick::Update();

		if (element_canMove::GetMatchShadow(getHitBoxCenterIndex()) != nullptr)
		{
			// 1 回使うとしばらく使えなくなる。
			if (charge > 0)
			{
				charge = 0;
			}
		}
		else
		{
			charge++;
		}
	}
}
void gimmick_warpPoint::Draw()
{
	if (getExistence())
	{
		if (!getFullCharge())
		{
			SetDrawBright(64, 64, 64);
		}
		DrawGraph(createDisplayValue().x, createDisplayValue().y, GH_warpPoint, TRUE);
		
		SetDrawBright(255, 255, 255);
	}
}
void gimmick_warpPoint::DrawSurface()
{
	if (hitPl)
	{
		SetDrawBlendMode(DX_BLENDMODE_ADD, 256);
		DrawGraph(createDisplayValue().x, createDisplayValue().y - CELL_SIZE, GH_effect, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

void gimmick_warpPoint::CreateWarpDestination()
{
	if (!pInstances.empty())
	{
		// 何もしないと座標順に並んでしまうのでシャッフル（オブジェクト単位）
		for (int i = 0; i < pInstances.size(); i++)
		{
			int randIdx = rand() % pInstances.size();

			if (i != randIdx)
			{
				gimmick_warpPoint* tmp = pInstances[i];
				pInstances[i] = pInstances[randIdx];
				pInstances[randIdx] = tmp;
			}
		}
		for (int i = 0; i < pInstances.size(); i++)
		{
			pInstances[i]->vec_dest.clear();

			for (int j = 0; j < pInstances.size(); j++)
			{
				// 自分自身の座標はワープ先候補に入れない。
				if (i != j)
				{
					pInstances[i]->vec_dest.push_back(pInstances[j]->getHitBoxCenterIndex());
				}
			}
		}
	}
}

void gimmick_warpPoint::setHitBoxIndex(vector2D<int> _idx)
{
	gimmick::setHitBoxIndex(_idx);

	board_main* pBoard = board_main::getInstance();

	if (pBoard != nullptr)
	{
		pBoard->DigStartPoint(_idx, false, 3);
	}
}

gimmick_warpPoint* gimmick_warpPoint::getInstanceFromIndex(vector2D<int> _idx)
{
	for (int i = 0; i < pInstances.size(); i++)
	{
		if (pInstances[i]->getExistence())
		{
			vector2D<int> chkIdx = pInstances[i]->getHitBoxCenterIndex();

			if (chkIdx == _idx)
			{
				return pInstances[i];
			}
		}
	}
	return nullptr;
}
vector2D<int> gimmick_warpPoint::getDestinationIndex() 
{
	return vec_dest[rand() % vec_dest.size()]; 
}