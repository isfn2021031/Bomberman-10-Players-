#include "../h/input_npc_easy.h"

input_npc_easy::input_npc_easy(int code, int id) :input_npc(code, id)
{
	patternStr = "easy";
	rates.bomb = 0.3;
	rates.update_avoidance = 4;
}