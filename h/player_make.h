#pragma once
#include <vector>
#include "../h/player_base.h"
#include "../h/board_base.h"
#include "../h/gimmickMgr.h"
#include "../h/bombMgr.h"

class player_make :public player_base
{
public:
	player_make						();
	~player_make					();

	void			Update			();
	void			Draw			();

	void			setBoard		(board_base* mk_b);
	void			setGimmickMgr	(gimmickMgr* gM)			{ gimmicks = gM; }

private:
	board_base*		pBoard = nullptr;
	gimmickMgr*		gimmicks = nullptr;
	bombMgr			bombs;
	int				alphaCnt = 0;

	bool			checkHit		(vector2D<pixelPosi> p);
	static int		GH_Frame;
	vector<int>		GH_types;
};