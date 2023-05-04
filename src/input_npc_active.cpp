#include "../h/input_npc_active.h"
#include "../h/dangerZoneForecast.h"

input_npc_active::input_npc_active(int code, int id) :input_npc_normal(code, id)
{
	patternStr = "active";
	rates.eyes = BOARD_WD;
	rates.escape *= 1.15;
	rates.bomb = 0.2;
}
int input_npc_active::createDirection(int realD)
{
	ctrlX = getRandData() % 2;

	int rv = (getRandData() % 2) ? Direction_Down : Direction_Up;

	if (ctrlX)
	{
		rv = (getRandData() % 2) ? Direction_Left : Direction_Right;
	}
	if (centerIdx_ptr != nullptr)
	{
		vector2D<int> newIdx = *centerIdx_ptr + getDirectionSigns(rv);
		board_main* pBoard = board_main::getInstance();

		if (pBoard != nullptr)
		{
			if (!pBoard->checkGroundFromIndex(newIdx) || dangerZoneForecast::GetInstance().getDangerZone(newIdx))
			{
				return Direction_None;
			}
		}
	}
	return rv;
}