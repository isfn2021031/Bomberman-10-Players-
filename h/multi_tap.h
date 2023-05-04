#pragma once
#include "../h/boardMenu.h"

enum
{
	multiTap_1,
	multiTap_2,
	multiTap_3,
	multiTap_4,
	multiTap_5,
	multiTap_6,
	multiTap_7,
	multiTap_8,
	multiTap_9,
	multiTap_Left,
	multiTap_0,
	multiTap_Right,

	multiTap_MAX,
	multiTap_None,
};

class multi_tap
{
public:
	multi_tap					();
	~multi_tap					();
	void				Update	();
	void				Draw	();

private:
	const vector<char>	InputChar[multiTap_MAX] = 
	{
		{'0', '1','2', '3', '4', '5', '6', '7', '8', '9'},
		{'A', 'B', 'C'},
		{'D', 'E', 'F'},
		{'G', 'H', 'I'},
		{'J', 'K', 'L'},
		{'M', 'N', 'O'},
		{'P', 'Q', 'R', 'S'},
		{'T', 'U', 'V'},
		{'W', 'X', 'Y' ,'Z'},
	};
	boardMenu*			mBoard = nullptr;
	graphicParam*		GH_multiTap = nullptr;
	int					playerID = -1;
};