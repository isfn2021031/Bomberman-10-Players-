#pragma once
#include "../h/suddenDeath.h"

class suddenDeath_rota :public suddenDeath
{
public:
	suddenDeath_rota		(int, bool se = true);
	void	Update			()override;

private:
	void	createSetIndex	()override;
};