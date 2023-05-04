#pragma once
#include "../h/element.h"

class darkness
{
public:
	void				Update			();
	void				Draw			();
	void				setPlayerHole	(int id, vector2D<pixelPosi> px);
	void				setExistence	(int id, bool flag);
	static darkness*	getInstance		();

private:
	darkness							();
	virtual				~darkness		();

	vector2D<pixelPosi>	holePositions[PLAYER_MAX];
	bool				Existence[PLAYER_MAX] = { false };
	int					Screen_darkness = -1;
	int					Screen_brightness = -1;
	
	static darkness*	itselfptr;
	static int			GH_brighthole;
};