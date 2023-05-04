#pragma once
#include "../h/menuCursorMgr.h"

class menuCursorMgr_Y :public menuCursorMgr
{
public:
	menuCursorMgr_Y	(int _max);
	void	Draw	(int px, int py)override;

protected:
	int		getSign	()override;
};