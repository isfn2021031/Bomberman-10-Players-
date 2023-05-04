#include "DxLib.h"
#include "../h/timer.h"
#include "../h/debug.h"
#include "../h/global.h"
#include "../h/configLoader.h"
#include "../h/graphicMgr_common.h"

#define FC_TIME_1_MIN			(FC_TIME_1_SEC * 60)
#define FC_TIME_1_SEC			(60)

graphicParam		timer::GH_timer;
graphicParam		timer::GH_colon;
//graphicParam*		timer::GH_numbers = nullptr;

timer::timer()
{
	configLoader cL(FILEPATH_CONF, CONFIG_MAX);
	configTime = FC_TIME_1_MIN * (cL.getConfig(CONFIG_Time) + 1);
	remainingTime = configTime;

	if (GH_timer.handle == -1)
	{
		GH_timer = LoadGraphParam("textures/timer/timer.png");
	}
	if (GH_colon.handle == -1)
	{
		GH_colon = LoadGraphParam("textures/timer/colon.png");
	}
}

// 時間関係処理（全オブジェクト共通の静的関数）
void timer::Update()
{
	FrameCnt++;
	remainingTime = (configTime - FrameCnt);
}
void timer::Draw()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 192);

	int x = WINDOW_WIDTH / 2;
	int y = CELL_SIZE / 2;

	DrawRotaGraph(x, y, 1.0, 0.0, GH_timer.handle, TRUE);
	
	int v = remainingTime / FC_TIME_1_SEC;
	if (v < 0)
	{
		v = 0;
	}
	drawMinute(x + GH_timer.width * 0.1, y, 1.0, convertSecondToMinute(v));
	
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

// 例:120秒 →「02:00」
void timer::drawMinute(int x, int y, double ExRate, minute value)
{
	graphicMgr_common::GetInstance().DrawValue(x, y, 1.0, value.sec);
	DrawRotaGraph(x, y, ExRate, 0.0, GH_colon.handle, TRUE);
	x -= (GH_colon.width * ExRate);
	graphicMgr_common::GetInstance().DrawValue(x, y, 1.0, value.min);
}

bool timer::getStart(int sec)
{
	return (remainingTime / FC_TIME_1_SEC) <= sec; 
}

timer::minute timer::convertSecondToMinute(int sec)
{
	minute rv;

	while (sec >= 60 && ProcessMessage() == 0)
	{
		sec -= 60;
		rv.min++;
	}
	rv.sec = sec;

	return rv;
}