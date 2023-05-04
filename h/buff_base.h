#pragma once
#include "../h/RGB.h"
#include "../h/effect_rotaGraph3.h"

enum
{
	BuffType_FireUp,
	BuffType_SpeedUp,
	BuffType_BombUp,

	BuffType_Invincibility,
	BuffType_Skull,
	BuffType_Countdown,

	BuffType_MAX,
	BuffType_None,
};

class buff_base
{
public:
	buff_base							(int fcMAX = 600);
	~buff_base							();

	void				Update			();
	virtual void		Draw			(int dx, int dy);
	virtual void		Finalize		()				{}
	void				setExistence	(bool f);
	void				setPattern		(int pt);
	void				setFrameCnt		(int c)			{ frameCnt = c; }
	void				setAuraColor	(RgbData rgb)	{ auraColor = rgb; }
		
	bool				getExistence	()				{ return existence; }
		
	int					getAfterCnt		()				{ return afterCnt; }
	int					getPattern		()				{ return pattern; }
	int					getBuffSpeed	()				{ return buffSpeed; }
	int					getSecondLeft	()				{ return second_left; }
	int					getFrameCnt		()				{ return frameCnt; }
	int					getFrameCnt_left()				{ return frameCnt_left; }
	int					getFrameCnt_MAX	()				{ return frameCnt_MAX; }
	int					getAlphaParam	()				{ return int(255 * ((double)frameCnt_left / frameCnt_MAX)); }

protected:
	RgbData				auraColor = { YELLOW_R, YELLOW_G, YELLOW_B };
	int					second_left = 0;
	int					second_old = 0;
	int					buffSpeed = 0;
	int					pattern = 0;
		
	virtual void		bf_main() {}

private:
	graphicParam		param;
	effect_rotaGraph3*	pEff_values = nullptr;

	bool				existence = false;

	int					afterCnt = 0;
	int					frameCnt = 0;
	int					frameCnt_left = 0;
	int					frameCnt_MAX = 0;
	int					auraIdx = 0;

	static int*			GH_aura;

	void				drawBuffIcon(int dx, int dy, int b);
};