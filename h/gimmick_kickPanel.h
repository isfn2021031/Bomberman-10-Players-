#pragma once
#include "../h/gimmick.h"

class gimmick_kickPanel :public gimmick
{
public:
	gimmick_kickPanel();

	void Update();
	void Draw();

private:
	static int GH_panels[Direction_MAX];
};