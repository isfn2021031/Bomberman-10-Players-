#include "DxLib.h"
#include "../h/dialogue.h"
#include "../h/soundMgr.h"

int dialogue::GH_win = -1;

dialogue::dialogue(string str)
{
	txAnime_ptr = new textAnimation(str);
	soundMgr::GetInstance().Play(SE_Pause);

	if (GH_win == -1)
	{
		GH_win = LoadGraph("textures/win.png");
	}
}
dialogue::~dialogue()
{
}

int dialogue::Update()
{
	if (finishAnimationCnt)
	{
		if (finishAnimationCnt > finishAnimationMAX)
		{
			return 1;
		}
		finishAnimationCnt++;
	}
	else
	{
		if (txAnime_ptr != nullptr)
		{
			if (txAnime_ptr->Update())
			{
				finishAnimationCnt = 1;
			}
		}
	}
	return 0;
}
void dialogue::Draw()
{
	double exrate = 1.0;

	if (finishAnimationMAX && finishAnimationCnt)
	{
		exrate = ((double)finishAnimationMAX - (double)finishAnimationCnt) / (double)finishAnimationMAX;
	}
	int cx = WINDOW_WIDTH / 2;
	int cy = WINDOW_HEIGHT / 2;
	DrawRotaGraph(cx, cy, exrate, 0.0, GH_win, TRUE);

	if (!finishAnimationCnt)
	{
		if (txAnime_ptr != nullptr)
		{
			txAnime_ptr->Draw(WINDOW_WIDTH * 0.3 + 20, WINDOW_HEIGHT * 0.45);
		}
	}
}