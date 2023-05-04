#pragma once
#include "../h/itemPanel.h"
#include "../h/graphicMgr_bomberman.h"

#define HANDI_SIZE		(10)

class handicap
{
public:
	handicap(int id);
	~handicap();

	void					Draw(int x, int y);
	void					DrawCursor();
	void					setIdx(int n)			{ idx = n; }

	bool					load();
	bool					add(int itemType);
	bool					sub();

	int						get(int i)				{ return items[i]; }

private:
	graphicMgr_bomberman	graphMgr;
	string					FileName;
	vector2D<int>			cp = { 0,0 };
	int						idx = 0;
	int						items[HANDI_SIZE] = { 0 };
	int						playerID = 0;
};