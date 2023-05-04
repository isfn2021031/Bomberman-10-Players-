#include "DxLib.h"
#include "../h/menuCursorMgr_XdispSingle.h"
#include "../h/soundMgr.h"

menuCursorMgr_XdispSingle::menuCursorMgr_XdispSingle(int _max) :menuCursorMgr_X(_max)
{
}
menuCursorMgr_XdispSingle::~menuCursorMgr_XdispSingle()
{
}
void menuCursorMgr_XdispSingle::Draw(int px, int py)
{
	DrawFormatString(px, py, GetColor(0, 0, 0), labelStr.c_str());
	DrawFormatString(px, py + 50, GetColor(0, 0, 0), cursorStr[cursor].c_str());
}
