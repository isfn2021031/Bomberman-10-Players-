#pragma once
#include "../h/input_key.h"
#include "../h/input_network.h"

class input_networkServer :public input_network
{
public:
	input_networkServer						(int id = 0);
	void						Update		()override;
	void						Draw		()override;

private:
	static int					GH_buttonTest;
	static int					GH_buttonTest_arrows[Direction_MAX];
	static int					GH_buttonTest_buttons[3];
};