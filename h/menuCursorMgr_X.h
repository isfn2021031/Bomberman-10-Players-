#pragma once
#include "../h/menuCursorMgr.h"

class menuCursorMgr_X :public menuCursorMgr
{
public:
	menuCursorMgr_X						(int _max);
	virtual			~menuCursorMgr_X	();
	virtual void	Draw				(int px, int py)override;

protected:
	int				getSign				()override;
};