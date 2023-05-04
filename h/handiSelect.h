#pragma once
#include "../h/handicap.h"
#include "../h/boardMenu.h"

class handiSelect
{
public:
	handiSelect						();
	virtual			~handiSelect	();

	void			Update			();
	void			Draw			();

	bool			checkFinished	() { return playerID < 0 || playerID >= PLAYER_MAX; }

private:
	handicap*		handi[PLAYER_MAX] = { nullptr };
	boardMenu*		mBoard = nullptr;
	int				playerID = 0;
};