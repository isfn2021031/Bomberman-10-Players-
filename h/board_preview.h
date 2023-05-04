#pragma once
#include "../h/board_main.h"

class board_preview :public board_main
{
public:
	board_preview					(int stgID, int cStyle);
	void	Draw					()override;

private:
	void	DrawPlayersUI			()override {}
};