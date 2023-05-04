#pragma once
#include "../h/player_base.h"

class player_ctr :public player_base
{
public:
	player_ctr										(int id);
	virtual					~player_ctr				()override;
	
private:
	int						ctr = 0;
	void					newNonPlayerCharacter	(int _type);
};