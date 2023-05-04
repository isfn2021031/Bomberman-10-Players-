#pragma once
#include "../h/player_ctr.h"
#include "../h/bombMgr.h"

// ‚Ý‚»ƒ{ƒ“
class badBomber :public player_ctr
{
public:
	badBomber												(int id, input_base* kI_ptr, bombMgr* _bombs);
	void						Update						();
	void						Draw						();
	void						Finalize					();

	void						setExistence				(bool flag)override;
	void						setHitBoxIndex				(vector2D<int> _idx)override;

	bool						getLeaving					()				{ return leaving; }

private:
	bombMgr*					pBombs = nullptr;
	bool						RangeOfMovement[BOARD_HEIGHT][BOARD_WIDTH] = { false };
	bool						starting = false;
	
	int							subSign = 1;
	int							forcedDirection = Direction_None;
	int							waitCnt = 0;
	int							chargeCnt = 0;
	bool						leaving = false;
	bool						finalLeave = false;

	int							createDirection				(vector2D<int> _idx);
	int							createSubSign				(int n, int max) { return (n < (max / 2)) ? -1 : 1; }
	bool						checkHit					(vector2D<pixelPosi> p)override;
};