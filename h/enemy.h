#pragma once
#include "../h/element_canMove.h"
#include "../h/elementMgr.h"

#define ENEMY_ANIME_MAX	(6)

enum
{
	Enemy_Ballom,
	Enemy_ONeal,
	Enemy_Dahl,
	Enemy_Minvo,
	Enemy_Doria,
	Enemy_Ovape,
	Enemy_Pass,
	Enemy_Pontan,

	Enemy_MAX,
};

class enemy :public element_canMove
{
public:
	enemy							();
	void				Update		()override;
	void				Draw		();
	static void			DrawEnemy	(int x, int y, int type, int animation, bool fire = false);
	static void			SetPtrItems	(elementMgr* pi)			{ ptr_items = pi; }
	void				setType		(int t)override;
	void				setExistence(bool f)override;

private:
	static elementMgr*	ptr_items;
	bool				fired = false;
	static int			GH_enemies[Enemy_MAX][ENEMY_ANIME_MAX];
	static int			GH_firedEnemies[Enemy_MAX][ENEMY_ANIME_MAX];
	static int			Convert_enemy_to_item(int enemyId);
};