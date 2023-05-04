#pragma once
#include "../h/element_canMove.h"

#define ELEM_ANIME_MAX			(4)

#define RANGE_THROW				(CELL_SIZE * 5 - 1)
#define RANGE_PUNCH				(CELL_SIZE * 3 - 1)

enum
{
	pushType_stop,
	pushType_slide,
	pushType_float,

	pushType_MAX
};

struct elemPushInfo
{
	int			playerID = -1;
	int			direction = 0;
	int			type = pushType_stop;		// 0:なし、1:キック、2:パンチ＆投げ
	int			cntr = 2;
	int			frameCnt = 0;
	pixelPosi	range = 0;
};

class element_canThrow :public element_canMove
{
public:
	element_canThrow						();
	void					Update			();
	void					Draw			();

//////////////////////////////////////////////////	getter	///////////////////////////////////////////////////////////////	
	elemPushInfo			getKickInfo		()					{ return pushState; }
	bool					getPass			()					{ return pass_real; }
	bool					getLand			()					{ return land; }
	bool					getCanKick		()					{ return canKick; }
	virtual int				getCoolTime		()					{ return 0; }

//////////////////////////////////////////////////	setter	///////////////////////////////////////////////////////////////	
	void					setHold			(bool _hold);
	void					setPushState	(elemPushInfo ps);
	void					setHitBoxIndex	(vector2D<int> _idx)override;

	bool					Stop			();

protected:
	struct
	{
		shadowParam*		target_shadow;
		bool				lock_on = false;
		int					targetID = -1;
		int					direction_old = 0;
	}tracingParam;
	
	elemPushInfo			pushState;

	bool					pass = false;											// 当たり判定無効
	bool					pass_real = false;										
	bool					canKick = true;											// キック可能か
	bool					moving = false;
	bool					reflection = false;
	bool					land = true;
	bool					hold = false;
	bool					hitBomb = false;

	void					update_main				();
	void					update_ignore			()override;
	bool					update_slide			(vector2D<pixelPosi> speed);
	bool					update_float			(vector2D<pixelPosi> speed);

	virtual bool			move_noSupport			(vector2D<pixelPosi> newPosi);
	virtual bool			pushMove				(vector2D<pixelPosi> newPosi);
	virtual bool			checkHit				(vector2D<pixelPosi> p)override;
	virtual bool			checkHitBomb			(vector2D<pixelPosi> p);		// マップ上の爆弾との当たり判定

	int						checkKickPanelDirection	(vector2D<pixelPosi> p);
	int						searchNearestPlayerID	(pixelPosi max);

private:
	static int				GH_flag[ELEM_ANIME_MAX];

	bool					pushMoveX				(vector2D<pixelPosi>& pPixel, pixelPosi n);
	bool					pushMoveY				(vector2D<pixelPosi>& pPixel, pixelPosi n);
	bool					checkMatchBombIndex		(vector2D<int> _idx)override;
	static int				CalcDirectionFrom2Point	(vector2D<int> idx1, vector2D<int> idx2);
};