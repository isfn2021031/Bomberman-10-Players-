#include "DxLib.h"
#include "../h/menuCursorMgr_X.h"
#include "../h/soundMgr.h"

menuCursorMgr_X::menuCursorMgr_X(int _max) :menuCursorMgr(_max)
{
	BoxRange = { 128,32 };
}
menuCursorMgr_X::~menuCursorMgr_X()
{
}
void menuCursorMgr_X::Draw(int px, int py)
{
	for (int i = 0; i < cursorMAX; i++)
	{
		if (!cursorStr[i].empty())
		{
			int bright = (i == cursor) ? 255 : 128;
			SetDrawBright(bright, bright, bright);

			int dx = px + (BoxRange.x * 1.1) * i;
			DrawExtendGraph(dx, py, dx + BoxRange.x, py + BoxRange.y, GH_mbox, TRUE);

			if (GH_accents[i] != -1)
			{
				DrawGraph(dx + (BoxRange.x - 70), py + 1, GH_accents[i], TRUE);
			}
			SetDrawBright(255, 255, 255);
			DrawFormatString(dx + 20, py + 4, GetColor(32, 64, 128), cursorStr[i].c_str());
		}
	}
}

int menuCursorMgr_X::getSign()
{
	int old = cursor;
	switch (kI->getDirectionEdge())
	{
	case Direction_Down:    return 1;
	case Direction_Up:      return -1;
	case Direction_Right:   cursor++; break;
	case Direction_Left:    cursor--; break;
	default:                break;
	}
	normalization();

	if (cursor != old)
	{
		soundMgr::GetInstance().Play(SE_Select);
	}

	if (kI->getButtonEdge(actButton_B))
	{
		return -1;
	}
	if (kI->getButtonEdge(actButton_A))
	{
		return 1;
	}
	return 0;
}
