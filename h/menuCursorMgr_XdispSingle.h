#pragma once
#include "../h/menuCursorMgr_X.h"

class menuCursorMgr_XdispSingle :public menuCursorMgr_X
{
public:
	menuCursorMgr_XdispSingle						(int _max);
	virtual				~menuCursorMgr_XdispSingle	();
	virtual void		Draw						(int px, int py)override;
};