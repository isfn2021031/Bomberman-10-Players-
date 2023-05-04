#pragma once
#include "../h/parameter.h"

struct statusParam
{
	bool			canPunch = false;
	bool			softBlockPass = false;

	int				firePower = 0;
	int				bombCapacity = 0;
	int				speed = 0;
	int				bombType = 0;
	int				kick_bombPass = 0;
	int				throw_lineBomb = 0;
	int				shield_change_rocket = 0;
};
struct statusParamMgr
{
	parameter*		lifeCnt = nullptr;
	parameter*		firePower = nullptr;
	parameter*		bombCapacity = nullptr;
	parameter*		speed = nullptr;
		
	bool			canPunch = false;
	bool			softBlockPass = false;

	int				bombType = 0;
	int				kick_bombPass = 0;
	int				powerGlove_lineBomb = 0;
	int				shield_change_rocket = 0;

	statusParamMgr						();
	~statusParamMgr						();

	void			setDefault			();
	void			setPowerFreaks		();
	void			setParameter		(statusParam param);

	statusParam		getParameter		();
};