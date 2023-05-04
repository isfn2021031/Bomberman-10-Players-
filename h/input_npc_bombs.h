#pragma once
#include "../h/input_npc.h"

class input_npc_bombs :public input_npc
{
public:
	input_npc_bombs(int code, int id);
	void			Update()override;
	
private:
	const int		Interval = 600;
	int				old_flagCnt = 0;

	bool			setButton_bomb(bool f = false)override;
	npcFlags		createFlags()override;
};