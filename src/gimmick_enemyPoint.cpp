#include "../h/gimmick_enemyPoint.h"
#include "../h/board_main.h"
#include "../h/board_withGimmick.h"
#include "../h/soundMgr.h"

enemyMgr* gimmick_enemyPoint::Enemies_ptr = nullptr;
int gimmick_enemyPoint::GH_point = -1;

gimmick_enemyPoint::gimmick_enemyPoint()
{
	if (GH_point == -1)
	{
		GH_point = LoadGraph("textures/gimmicks/enemyPoint.png");
	}
}
gimmick_enemyPoint::~gimmick_enemyPoint()
{

}

void gimmick_enemyPoint::Update()
{
	if (getExistence())
	{
		gimmick::Update();

		// マリオと同じく入り口に張り付いていると敵が湧かない。
		if (hitPl || element_canMove::GetMatchShadow(getHitBoxCenterIndex()) != nullptr)
		{
			// 1 回使うとしばらく使えなくなる。
			if (charge > 0)
			{
				charge = 0;
			}
		}
		else if (Enemies_ptr != nullptr)
		{
			if (charge >= FullCharge)
			{
				if (Enemies_ptr->setNewElement(getHitBoxCenterIndex()) != nullptr)
				{
					charge = 0;
					soundMgr::GetInstance().Play(SE_Reverse);
				}
			}
			else
			{
				charge++;
			}
		}
	}
}
void gimmick_enemyPoint::Draw()
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
				
				if (charge < FullCharge * 0.75)
				{
					SetDrawBright(64, 64, 64);
					DrawGraph(drawX, drawY, GH_point, TRUE);
					SetDrawBright(255, 255, 255);
				}
				else
				{
					DrawGraph(drawX, drawY, GH_point, TRUE);
					for (int i = 0; i < getFlashRepetitions(50); i++)
					{
						DrawGraph(drawX, drawY, GH_point, TRUE);
					}
					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
				}
			}
		}
	}
}