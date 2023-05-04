#include "DxLib.h"
#include "../h/buff_base.h"
#include "../h/global.h"
#include "../h/graphicMgr_common.h"

int* buff_base::GH_aura = nullptr;

buff_base::buff_base(int fcMax)
{
	frameCnt_MAX = fcMax;

	if (GH_aura == nullptr)
	{
		GH_aura = new int[6];
		LoadDivGraph("textures/bomberman/buffAura.png", 6, 2, 3, 64, 90, GH_aura);
	}
}
buff_base::~buff_base()
{
	if (pEff_values != nullptr)
	{
		delete pEff_values;
	}
}

void buff_base::Update()
{
	if (existence)
	{
		if (frameCnt % 3 == 0)
		{
			auraIdx = (++auraIdx) % 6;
		}
		if (pEff_values != nullptr)
		{
			pEff_values->Update();
		}
		afterCnt++;
		second_old = second_left;

		frameCnt_left = (frameCnt_MAX - frameCnt);
		second_left = frameCnt_left / 60;				// 60‚ÅŠ„‚é‚Æ‚¾‚¢‚½‚¢‚P•b‚­‚ç‚¢‚É‚È‚éB

		// ŽžŠÔŒo‰ß‚ÅŽ©•ªŽ©g‚ðÁ‚·B
		if ((frameCnt++ > frameCnt_MAX))
		{
			existence = false;
		}
		bf_main();
	}
}
void buff_base::Draw(int dx, int dy)
{
	if (existence)
	{
		SetDrawBright(auraColor.r, auraColor.g, auraColor.b);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, getAlphaParam() * 0.5 + 32);
		DrawRotaGraph(BASIS_PX + dx + CELL_SIZE * 0.5, dy - 10, 1.0, 0.0, GH_aura[auraIdx], TRUE);
		
		SetDrawBright(255, 255, 255);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		drawBuffIcon(dx, dy, pattern);
	}
}

void buff_base::drawBuffIcon(int dx, int dy, int b)
{
	if (pEff_values != nullptr)
	{
		int w = param.width * 0.75;
		int dx2 = dx + param.width * 0.5;
		int dy2 = dy + param.height * 0.5;

		switch (b)
		{
		case BuffType_Invincibility:
			dx2 -= w;
			dy2 += param.height * 0.8;

		case BuffType_FireUp:
		case BuffType_SpeedUp:
		case BuffType_BombUp:
			pEff_values->Draw(dx2 + w * b + CELL_SIZE * 0.9, dy2 - CELL_SIZE * 1.25);
			break;

		default:
			break;
		}
	}
}

void buff_base::setPattern(int pt)
{
	pattern = pt; 

	if (pEff_values != nullptr)
	{
		delete pEff_values;
	}
	param = graphicMgr_common::GetInstance().getBuffHandle(pattern);
	pEff_values = new effect_rotaGraph3(param, 0.05);
}
void buff_base::setExistence(bool f)
{
	existence = f;
	afterCnt = 0;
}