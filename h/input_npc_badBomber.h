#pragma once
#include "../h/input_npc.h"

class input_npc_badBomber :public input_npc
{
public:
	input_npc_badBomber					(int code, int id);
	bool				getButtonEdge	(int n)override;
	int					getDirection	()override;

private:
	int afterBombCnt_base = 0;
	int afterBombCnt = 0;
};