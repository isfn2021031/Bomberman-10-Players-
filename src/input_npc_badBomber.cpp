#include "DxLib.h"
#include "../h/input_npc_badBomber.h"

input_npc_badBomber::input_npc_badBomber(int code, int id) :input_npc(code, id)
{
	rates.turn = 0.1;
	rates.punch = 0.0;
	afterBombCnt_base = getRandData() % (6 + id);
	direction = rand() % Direction_MAX;
}

int input_npc_badBomber::getDirection()
{
	return (--afterBombCnt) ? direction : Direction_None;
}

bool input_npc_badBomber::getButtonEdge(int n)
{
	if (n == actButton_A)
	{
		if (getRandData() % 4 == 0)
		{
			if (!afterBombCnt)
			{
				direction = getRandData() % Direction_MAX;
			}
			afterBombCnt = afterBombCnt_base;
			return true;
		}
	}
	return input_npc::getButtonEdge(n);
}