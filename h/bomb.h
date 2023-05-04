#pragma once
#include "../h/element_canThrow.h"

#define BOMB_MAX				(10)
#define BLAST_POWER_MAX			(8)

#define FC_BOMB_CHARGE			(100)
#define FC_BOMB_COOLTIME		(150)
#define FC_BOMB_STARTUP			(60)
#define FC_BOMB_BLAST			(20)
#define FC_BOMB_TOTAL			(FC_BOMB_COOLTIME + FC_BOMB_BLAST)

#define FC_FIRE_SOFT_BLOCK		(FC_BOMB_BLAST + 3)
#define FC_INFINITE				(9999999)

enum
{
	BombType_Normal,
	BombType_Power,
	BombType_Infinite,
	BombType_Dangerous,
	BombType_RemoteControl,
	BombType_RemoteControl_momentary,
	BombType_Pierce,
	BombType_SharpPierce,
	BombType_Rubber,
	BombType_Tracing,
	BombType_Mine,
	BombType_DangerousMine,
	BombType_Heavy,
	BombType_Color,
	BombType_Dynamite,

	BombType_Quick,
	BombType_Slow,
	BombType_Mix,

	BombType_MAX,

	BombType_RandomCoolTime,
	BombType_Ring,
	BombType_MineRush,
	BombType_HeavyRush,
	BombType_DangerousRush,
	BombType_DynamiteRush,

	BombType_None,
};

struct bombStatus
{
	int				playerID = -1;
	int				power = 1;
	int				bombType = BombType_Normal;
	int				blastTime = FC_BOMB_BLAST;
	int				blastType = 0;
	int				blastSpeed = 1;
	int				coolTime = FC_BOMB_COOLTIME;
};

class bomb :public element_canThrow
{
public:
	bomb									();
	void					Update			();
	void					UpdateExistences();
	void					Draw			();

	void					Explosion		();
	
//////////////////////////////////////////////////	getter	///////////////////////////////////////////////////////////////	
	int						getStep			()					{ return step; }
	virtual int				getCoolTime		()override			{ return bombInfo.coolTime; }

//////////////////////////////////////////////////	setter	///////////////////////////////////////////////////////////////	
	void					setBombID		(int id)			{ bombID = id; }
	void					setStatus		(bombStatus bs);
	void					setCoolTime		(int ct, bool changeAnimation = false);
	void					setRest			(bool _rest)		{ rest_setting = _rest; }
	void					setHitBoxIndex	(vector2D<int> _idx)override;

private:
	enum
	{
		Step_Startup,
		Step_CoolTime,
		Step_Explosion,
		
		Step_MAX
	};
	
	bombStatus				bombInfo;
	bool					rest_setting = false;												// ŽžŠÔŒo‰ß‚Å”š”­‚·‚é‚©
	bool					hitPlayer = false;
	bool					teamInvincibility_real = false;
	int						bombID = 0;
	int						step = 0;
	int						oldStep = 0;
	static int				GH_bomb[BombType_MAX][ELEM_ANIME_MAX];

	virtual bool			pushMove				(vector2D<pixelPosi> newPosi)override;
	
	bool					checkMatchBombIndex		(vector2D<int> _idx)override;
	virtual bool			checkHit				(vector2D<pixelPosi> p);

	int						createStep				();
};
