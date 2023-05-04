#pragma once
#include "../h/menuCursorMgr.h"

class menuCursorMgr_inGame :public menuCursorMgr
{
public:
	menuCursorMgr_inGame				(int max = 1, input_key* kIptr = nullptr, bool movLast = false);
	virtual		~menuCursorMgr_inGame	()override;
	int			Update					()override;
	void		Draw					(int px, int py)override;

	void		setRestInput			(bool f)override;

private:
	bool		finish = false;
	bool		decide = false;
};