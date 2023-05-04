#pragma once
#include "../h/element.h"

#define SUDDEN_PATTERN_MAX			(4)

class suddenDeath :public element
{
public:
	suddenDeath									(int pattern, bool se = true);
	~suddenDeath								();

	void						Initialize		(); 
	void						Update			();
	void						Draw			();
	
	void						setActive		(bool f) { active = f; }

	bool						getFlag			(vector2D<int> idx);
	bool						getActive		() { return active; }

protected:
	static const vector2D<int>	StartIndexlist[SUDDEN_PATTERN_MAX];
	static const int			DirectionsM[SUDDEN_PATTERN_MAX];
	static const int			DirectionsS[SUDDEN_PATTERN_MAX];

	vector2D<int>				setIndex = { 1,1 };
	vector2D<int>				startIndex = { 0,0 };

	bool						active = false;

	virtual void				createSetIndex	() = 0;
	
	bool						turn			(vector2D<int> _idx); 
	bool						setPressureBlock(vector2D<int> _idx);
	
	int							blockCnt = 0;
	int							subDirection = Direction_MAX;
	int							rate_update = 6;

private:

	bool						flag[BOARD_HEIGHT][BOARD_WIDTH] = { false };
	bool						flag_forecast[BOARD_HEIGHT][BOARD_WIDTH] = { false };
	bool						nowSet = false;
	bool						playSE = true;

	int							FrameCouter = 0;
	static int					GH_HurryUp;
};