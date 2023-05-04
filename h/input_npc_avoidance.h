#pragma once
#include "../h/input_npc.h"

class input_npc_avoidance :public input_npc
{
public:
	input_npc_avoidance(int code, int id);
	
private:
	//int getRiskCnt(vector2D<int> _idx, int cost = 0)override;
};