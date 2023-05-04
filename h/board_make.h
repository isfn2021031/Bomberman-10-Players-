#pragma once
#include "../h/board_withGimmick.h"
#include "../h/player_make.h"
#include "../h/gimmickMgr.h"

#define FPATH_DEF_STG	("bin/def_stg")

class board_make :public  board_withGimmick
{
public:
	board_make							();
	~board_make							();

	void					Update		()override;
	void					Draw		()override;
	
	void					setSave		(bool f) { save = f; }

private:
	player_make*			p1 = nullptr;
	bool					save = true;
	int						FrameCounterBlocks[BOARD_HEIGHT][BOARD_WIDTH] = { 0 };
};