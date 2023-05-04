#pragma once
#include "../h/gimmick.h"
#include "../h/enemyMgr.h"

class gimmick_enemyPoint :public gimmick
{
public:
	gimmick_enemyPoint();
	~gimmick_enemyPoint();
	void							Update		()override;
	void							Draw		()override;
	static void						SetEnemyMgr	(enemyMgr* enemies) { Enemies_ptr = enemies; }

private:
	static enemyMgr*				Enemies_ptr;
	const int						FullCharge = 360;
	int								charge = 0;
	static int						GH_point;
};