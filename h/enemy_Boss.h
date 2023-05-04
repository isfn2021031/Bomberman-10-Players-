#pragma once
#include "../h/enemy.h"

enum
{
	Boss_,
	Boss_MAX
};

class enemy_Boss :public enemy
{
public:
	enemy_Boss						();
	void				Update		()override;
	void				Draw		();
	static void			DrawBoss	(int x, int y, int type, int animation, bool fire = false);
	void				setType		(int t)override;
	void				setExistence(bool f)override;

private:
	bool				fired = false;
	int					hitPoint = 100;
	vector2D<int>		Size = { 3, 3 };
	static int			GH_Bosses[Boss_MAX][ENEMY_ANIME_MAX];
	static int			GH_firedBosses[Boss_MAX][ENEMY_ANIME_MAX];

	bool				checkHitBlast();
};