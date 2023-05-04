#pragma once
#include <string>
#include "../h/gimmick.h"

using namespace std;

class gimmick_roof :public gimmick
{
public:
	gimmick_roof(string _FileName);
	~gimmick_roof();

	void	DrawSurface();

	//////////////////////////////////////////////////	setter	///////////////////////////////////////////////////////////////	
	void	setHitBoxIndex(vector2D<pixelPosi> _idx)override;

private:
	int GH_roof = -1;
};