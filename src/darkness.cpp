#include "DxLib.h"
#include "../h/playerCounter.h"
#include "../h/bombMgr.h"
#include "../h/darkness.h"
#include "../h/RGB.h"

int darkness::GH_brighthole = -1;

darkness* darkness::itselfptr = nullptr;

darkness* darkness::getInstance() 
{
	if (itselfptr == nullptr)
	{
		itselfptr = new darkness;
	}
	return itselfptr; 
}

darkness::darkness()
{
	itselfptr = this;
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		Existence[i] = false;
		holePositions[i] = { 0,0 };
	}
	if (GH_brighthole == -1)
	{
		GH_brighthole = LoadGraph("textures/brighthole.png");
	}
	Screen_brightness = MakeScreen(WINDOW_WIDTH, WINDOW_HEIGHT, TRUE);
	Screen_darkness =	MakeScreen(WINDOW_WIDTH, WINDOW_HEIGHT, TRUE);
}
darkness::~darkness()
{
	itselfptr = nullptr;
	DeleteGraph(Screen_brightness);
	DeleteGraph(Screen_darkness);
}

void darkness::Update()
{
}
void darkness::Draw()
{
	int OldScreen = GetDrawScreen();
	
	// 明るい部分の描画
	SetDrawScreen(Screen_brightness);
	ClearDrawScreen();
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawBox(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GetColor(250, 0, 0), TRUE);

	for (int i = 0; i < playerCounter::GetInstance().getTotal(); i++)
	{
		if (Existence[i])
		{
			DrawRotaGraph
			(BASIS_PX + holePositions[i].x + 22, BASIS_PY + holePositions[i].y + 14, 1.0, 0.0, GH_brighthole, TRUE);
		}
	}
	// 暗い部分の描画
	SetDrawScreen(Screen_darkness);
	ClearDrawScreen();

	// Brightnessの赤要素をDarknessのアルファ値としてブレンドする
	GraphBlend(Screen_darkness, Screen_brightness, 255, DX_GRAPH_BLEND_RGBA_SELECT_MIX,
		DX_RGBA_SELECT_SRC_R,    // 出力結果の赤成分は			Darkness	の赤成分
		DX_RGBA_SELECT_SRC_G,    // 出力結果の緑成分は			Darkness	の緑成分
		DX_RGBA_SELECT_SRC_B,    // 出力結果の青成分は			Darkness	の青成分
		DX_RGBA_SELECT_BLEND_R   // 出力結果のアルファ成分は	Brightness	の赤成分
	);
	// 描画先を元に戻す
	SetDrawScreen(OldScreen);
	DrawGraph(0, 0, Screen_darkness, TRUE);
}
void darkness::setExistence(int id, bool flag)
{
	if (id >= 0 && id < PLAYER_MAX)
	{
		Existence[id] = flag;
	}
}
void darkness::setPlayerHole(int id, vector2D<pixelPosi> px)
{
	/*	for (int d = 0; d < Direction_MAX; d++)
	{
		vector2D<int> newIdx = idx;

		for (int k = 0; k < 3; k++)
		{
			if (!element::checkOffScreenIdx(newIdx))
			{
				values[newIdx.y][newIdx.x] -= 200;
			}
			newIdx = newIdx + getDirectionSigns(d);
		}
	}
	for (int d = 0; d < DiagonalDirection_MAX; d++)
	{
		vector2D<int> newIdx = { idx.x + getDirctionSigns2(d).x, idx.y + getDirctionSigns2(d).y };

		if (!element::checkOffScreenIdx(newIdx))
		{
			if (values[newIdx.y][newIdx.x] > 0)
			{
				values[newIdx.y][newIdx.x] -= 128;
			}
		}
	}
*/
	if (id >= 0 && id < PLAYER_MAX)
	{
		holePositions[id] = px;
	}
}