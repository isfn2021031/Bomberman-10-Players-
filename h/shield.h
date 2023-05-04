#pragma once
#include "../h/element.h"
#include "../h/input_base.h"

struct shieldData
{
	vector2D<int> idx = { 0,0 };
	int direction = 0;
	bool existence = false;
};

class shield :public element
{
public:
	shield							(int id);
	void				Update		();
	void				Draw		();
	virtual void		setExistence(bool flag)override;
	static bool			checkData	(vector2D<int> _idx, int _direction);

private:
	static shieldData	StaticData[PLAYER_MAX];
	static int			GH_shield[Direction_MAX];
};