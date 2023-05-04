#include <time.h>
#include "DxLib.h"

#include "../h/board_make.h"
#include "../h/debug.h"

#include "../h/soundMgr.h"
#include "../h/Scene_Menu.h"

board_make::board_make()
{
	p1 = new player_make();
	p1->setHitBoxIndex({ BOARD_WIDTH / 2, BOARD_HEIGHT / 2 });
	p1->setBoard(this);
	p1->setGimmickMgr(gimmicks);

	board_withGimmick::setEditFileName(FPATH_DEF_STG);
	LoadBoardToFile();
}
board_make::~board_make()
{
	if (save)
	{
		SaveBoardToFile();
	}
	delete p1;
}

void board_make::Update()
{
	p1->Update();
}
void board_make::Draw()
{
	board_withGimmick::Draw();

	if (gimmicks != nullptr)
	{
		gimmicks->DrawSurfaces();
	}
	if (gimmicks != nullptr)
	{
		gimmicks->DrawUsedAreas();
	}
	p1->Draw();
}