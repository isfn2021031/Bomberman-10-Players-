#pragma once
#include "../h/buff_base.h"

#define DEATH_COUNT_MAX		(20)

class buff_countdown :public buff_base
{
public:
	buff_countdown(int fcMAX = 1200);
	~buff_countdown();

	void			Draw(int dx, int dy)override;

private:
	static int		GH_counter;

	void			bf_main()override;
};