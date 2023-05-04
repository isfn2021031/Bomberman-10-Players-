#pragma once
#include "../h/suddenDeath.h"

class suddenDeath_normal :public suddenDeath
{
public:
	suddenDeath_normal(int, bool se = true);

private:	
	
	void			createSetIndex()override;
	void			normalization(vector2D<int>& idx);
};