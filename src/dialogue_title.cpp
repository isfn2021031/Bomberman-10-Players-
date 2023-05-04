#include "DxLib.h"
#include "../h/dialogue_title.h"

int dialogue_title::GH_titleArt = -1;

dialogue_title::dialogue_title() :dialogue("")
{
	if (GH_titleArt == -1)
	{
		GH_titleArt = LoadGraph("textures/win.png");
	}
	finishAnimationMAX = 60;
}
dialogue_title::~dialogue_title()
{
}

void dialogue_title::Draw()
{
	int cx = WINDOW_WIDTH / 2;
	int cy = WINDOW_HEIGHT / 2;
	DrawRotaGraph(cx, cy, 1.0, 0.0, GH_titleArt, TRUE);

	if (!finishAnimationCnt)
	{
		int dx1 = WINDOW_WIDTH * 0.3;
		int dy = WINDOW_HEIGHT * 0.45;
		//DrawFormatString(dx1 + 20, dy, GetColor(255, 255, 255), String_disp.c_str());
	}
}