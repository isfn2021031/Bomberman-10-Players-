#pragma once
#include "../h/input_key.h"

class input_key_badBomber :public input_key
{
public:
	input_key_badBomber							(int code, int id = 0);
	int					createSubDirection		()override;
};