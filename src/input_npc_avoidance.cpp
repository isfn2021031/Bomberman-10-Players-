#include "../h/input_npc_avoidance.h"
#include "../h/dangerZoneForecast.h"

input_npc_avoidance::input_npc_avoidance(int code, int id) :input_npc(code, id)
{
	patternStr = "avoidance";
	rates.eyes = BOARD_HD;
	rates.escape *= 1.15;
	rates.bomb = 0.2;
	rates.update_avoidance = 1;
}
/*int input_npc_avoidance::getRiskCnt(vector2D<int> _idx, int cost)
{
	return	dangerZoneForecast::GetInstance().getRiskCnt(_idx, cost);
}*/
