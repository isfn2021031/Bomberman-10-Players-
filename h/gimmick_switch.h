#pragma once
#include "../h/gimmick.h"

class gimmick_switch :public gimmick
{
public:
	gimmick_switch();
	void			Update();
	void			Draw();

private:
	static int		GH_switch[2];
};