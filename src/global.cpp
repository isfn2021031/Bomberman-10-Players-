#include "../h/global.h"

const vector2D<int> dirPositions[Direction_MAX] =
{
	{0,	1},
	{0,-1},
	{-1,0},
	{1,	0}
};
const vector2D<int> DirPositions2[DiagonalDirection_MAX] =
{
	{-1,-1},
	{1,-1},
	{-1,1},
	{1,	1}
};
const string arrows[Direction_MAX] = { "Å´","Å™","Å©","Å®" };

vector2D<int> getDirectionSigns(int d)
{
	if (d == Direction_None)
	{
		return { 0,0 };
	}
	return dirPositions[d];
}
vector2D<int> getDirctionSigns2(int d)
{
	return DirPositions2[d];
}

string getDirectionString(int d)
{
	return arrows[d];
}
int getReverseDirection(int d)
{
	switch (d)
	{
	case Direction_Down:	return Direction_Up;
	case Direction_Up:		return Direction_Down;
	case Direction_Left:	return Direction_Right;
	case Direction_Right:	return Direction_Left;
	default:				break;
	}
	return Direction_None;
}
