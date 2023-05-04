#include "../h/input_npc_normal.h"

input_npc_normal::input_npc_normal(int code, int id) :input_npc(code, id)
{
	patternStr = "normal";

	rates.eyes += rand() % 3;
	
	rates.bomb = 0.4;
	rates.remote = 1.0;
	rates.punch = 0.9;
	rates.turn = 0.1;
	rates.escape /= (rand() % 10) + 1;
}