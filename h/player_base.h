#pragma once
#include <vector>
#include "../h/playerCounter.h"
#include "../h/status.h"
#include "../h/element_canMove.h"
#include "../h/graphicMgr_bomberman.h"

class player_base :public element_canMove
{
public:
	player_base									(int _playerID);
	virtual					~player_base		();
	void					Update				();
	void					Draw				();
	virtual void			DrawPlayerName		();

	virtual void			setExistence		(bool flag)override;
	virtual void			setHitBoxPixel		(vector2D<pixelPosi> p)override;

protected:
	input_base*				kI = nullptr;
	element_canThrow*		pHoldBomb = nullptr;
	graphicMgr_bomberman	graphMgr;

	bool					movement = false;				// 歩行アニメーション
	bool					graphReverse = false;
	bool					nowSet = false;					// ボタンが押された
	
	const int				SupportCellSize = CELL_SIZE - 10;
	
	double					exRate = 1.0;
	virtual bool			checkWarp			(vector2D<int>)override { return false; }
	virtual bool			checkHit			(vector2D<pixelPosi> p) = 0;
	bool					pushBomb			(element_canThrow*& elemptr, elemPushInfo ps);

private:
	string					nameStr = "";
};