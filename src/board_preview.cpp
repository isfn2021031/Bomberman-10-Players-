#include "../h/board_preview.h"

board_preview::board_preview(int stgID, int cStyle) :board_main(stgID, cStyle)
{
	if (sudden != nullptr)
	{
		delete sudden;
		sudden = nullptr;
	}
	if (sudden_forecast != nullptr)
	{
		delete sudden_forecast;
		sudden_forecast = nullptr;
	}
	if (blastMgr != nullptr)
	{
		delete blastMgr;
		blastMgr = nullptr;
	}
}

void board_preview::Draw()
{
	board_main::Draw();
	
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 64);
	DrawBox(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, RGB_BLACK, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}