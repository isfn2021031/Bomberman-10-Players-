#pragma once

#include "../h/board_withGimmick.h"
#include "../h/bombBlastMgr.h"
#include "../h/itemMgr.h"
#include "../h/playerMgr.h"
#include "../h/suddenDeath.h"
#include "../h/timer.h"

#define FPATH_STG_FOLDER		("user/stg/")
#define FPATH_STG_PARAMETER		("bin/stg_prmtr")
#define StartPointType_MAX		(4)

class board_main : public board_withGimmick
{
public:
	board_main										(int stgID, int cStyle);// , bool _preview = false);
	virtual					~board_main				();	
	void					Update					()override;
	void					UpdateFlow				();
	void					Draw					()override;

	void					setGround				(vector2D<int>_idx)							{ cells[_idx.y][_idx.x].type = groundId; }
	void					setBlast				(vector2D<int>_idx, bombStatus bs);
	void					StartTimer				();
	void					disappearItem			(vector2D<int> idx)							{ items->setItems_Normal(); }
	void					scatterItems			(vector<int> history);

	bool					checkGroundFromIndex	(vector2D<int>_idx, bool withBomb = false);
	bool					getGameOver				()											{ return gameOver; }

	// 引数最後のフラグはボムの壁判定に使う
	int						getCanSetBlast			(vector2D<int>_idx, bombStatus bs, int _direction = Direction_None, bool chkBomb = false);
	itemMgr*				getItemMgr				()											{ return items; }
	bombBlastMgr*			getBlastMgr				()											{ return blastMgr; }
	static board_main*		getInstance				()											{ return pInstance; }

protected:
	timer*					timeMgr = nullptr;
	playerMgr*				pls = nullptr;
	itemMgr*				items = nullptr;
	bombBlastMgr*			blastMgr = nullptr;
	suddenDeath*			sudden = nullptr;
	suddenDeath*			sudden_forecast = nullptr;
	blastStatus				waitFrame[BOARD_HEIGHT][BOARD_WIDTH];

	bool					gameOver = false;
	bool					suddenF = false;
	bool					suddenF_forecast = false;

	int						stageID = 0;
	int						FrameCounterBlocks[BOARD_HEIGHT][BOARD_WIDTH] = { 0 };

	void					createStage				(int stgID);// , bool preview = false);
	void					createBeltZone2			();

	virtual void			DrawPlayersUI			();
	virtual void			placePlayers			()override;		// プレイヤーを new して、ポジショニングもする関数
	void					setSoftBlocks			();
	void					setMineBombs			();
	void					setRemoconBombs			();
	bool					setSubBlast_Cross		(vector2D<int>_idx, bombStatus bs);
	bool					setSubBlast_Dangerous	(vector2D<int>_idx, bombStatus bs, int range);
	bool					checkGameOver			();

private:
	static board_main*		pInstance;
	bool					pontanSudden = false;
	int						convertExplosionIdx		(int idx, bool end = false);
};