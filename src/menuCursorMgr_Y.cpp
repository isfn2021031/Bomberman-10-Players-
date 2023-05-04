#include "DxLib.h"
#include "../h/menuCursorMgr_Y.h"
#include "../h/soundMgr.h"

menuCursorMgr_Y::menuCursorMgr_Y(int _max) :menuCursorMgr(_max)
{
}
void menuCursorMgr_Y::Draw(int px, int py)
{
	//DrawFormatString(px, py,		GetColor(0, 0, 0), labelStr.c_str());
	//DrawFormatString(px, py + 50,	GetColor(0, 0, 0), cursorStr[cursor].c_str());
}
int menuCursorMgr_Y::getSign()
{
	int old = cursor;
	switch (kI->getDirectionEdge())
	{
	case Direction_Right:   return 1;
	case Direction_Left:    return -1;
	case Direction_Down:	cursor++; break;
	case Direction_Up:		cursor--; break;
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
