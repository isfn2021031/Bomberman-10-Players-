#pragma once
#include "../h/board_base.h"
#include "../h/bombMgr.h"
#include "../h/gimmickMgr.h"
#include "../h/enemyMgr.h"

class board_withGimmick :public board_base
{
public:
	board_withGimmick					();
	virtual			~board_withGimmick	();
	virtual void	Update				()override;
	virtual void	Draw				()override;

	virtual void	LoadBoardToFile		();
	void			SaveBoardToFile		();
	void			DigStartPoint		(vector2D<int> _idx, bool breakHard = true, int n = 2);

	void			setStartPattern		(int pattern)			{ positionPattern = pattern; }
	virtual void	setBombID			(vector2D<int> _idx, int id)override;
	static void		setEditFileName		(string fname)			{ EditFileName = fname; }
	gimmick*		getGimmick			(vector2D<int> _idx)	{ return gimmicks->getGimmick(_idx); }
	element*		getEnemy			(vector2D<int> _idx);

protected:
	gimmickMgr*		gimmicks = nullptr;
	enemyMgr*		enemies = nullptr;
	bombMgr*		bombs = nullptr;

	int				positionPattern = StartPattern_Normal;

	virtual void	placePlayers		();
	void			placeEnemies		();
	void			placeEnemies_sudden	();

private:
	static string	EditFileName;
	bool			config_enemies = false;
	
	int				bombIDs[BOARD_HEIGHT][BOARD_WIDTH] = { BombType_None };

};
