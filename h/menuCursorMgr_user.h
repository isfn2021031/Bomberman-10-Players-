#pragma once
#include "../h/menuCursorMgr.h"

class menuCursorMgr_user :public menuCursorMgr
{
public:
	menuCursorMgr_user										(int pID = 0, bool movLast = false);
	virtual							~menuCursorMgr_user		();

	int								Update					();
	void							Draw					(int px, int py)override;
	void							DrawGuideStr			(int x1, int y1, int x2, int y2)override;

	void							setCursorStr			(vector<string> strings)override;
	void							setDrawcursorNum		(int n)								{ dCursorNum = n; }

private:
	static vector<string>			LoadAndRefreshUserNames	();

	int								playerID = 0;
	int								dCursorNum = 3;
};