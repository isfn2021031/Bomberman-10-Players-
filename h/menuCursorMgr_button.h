#pragma once
#include "../h/menuCursorMgr.h"

class menuCursorMgr_button :public menuCursorMgr
{
public:
					menuCursorMgr_button(int max = 1);
	void			setPlusButtonID		(int id) { plusButtonID = id; }
	void			setMinusButtonID	(int id) { minusButtonID = id; }
	void			createCursor		()override;

private:
	int				plusButtonID = 1;
	int				minusButtonID = 0;
};