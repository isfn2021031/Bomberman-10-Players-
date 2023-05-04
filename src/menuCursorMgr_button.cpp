#include "DxLib.h"
#include "../h/menuCursorMgr_button.h"
#include "../h/soundMgr.h"

menuCursorMgr_button::menuCursorMgr_button(int max) :menuCursorMgr(max)
{

}
void menuCursorMgr_button::createCursor()
{
	if (kI->getButtonEdge(plusButtonID))
	{
		cursor++;
	}
	if (kI->getButtonEdge(minusButtonID))
	{
		cursor--;
	}
}