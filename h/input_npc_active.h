#pragma once
#include "../h/input_npc_normal.h"

class input_npc_active :public input_npc_normal
{
public:
	input_npc_active(int code, int id);
	
private:
	bool			ctrlX = true;

	int				createDirection(int realD)override;
};