#include "../h/suddenDeath_normal.h"

suddenDeath_normal::suddenDeath_normal(int pattern, bool se) :suddenDeath(pattern, se)
{
	rate_update = 6;
}
void suddenDeath_normal::createSetIndex()
{
	setIndex.x += getDirectionSigns(direction).x;
	
	if (getFlag(setIndex))
	{
		setIndex.y += getDirectionSigns(subDirection).y;
		normalization(setIndex);
	}
}

void suddenDeath_normal::normalization(vector2D<int>& _idx)
{
	if (_idx.x < 0 || _idx.x >= BOARD_WIDTH)
	{
		_idx.x = startIndex.x;
	}
	if (_idx.y < 0 || _idx.y >= BOARD_HEIGHT)
	{
		_idx.y = startIndex.y;
		active = false;
	}
}