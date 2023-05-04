#pragma once
#include <vector>
#include "../h/element_accessableBoard.h"

#define BURN_EFF_NUM	(5)

enum
{
	itemPanel_FireUp,
	itemPanel_SpeedUp,
	itemPanel_BombUp,

	itemPanel_FireDown,
	itemPanel_SpeedDown,
	itemPanel_BombDown,

	itemPanel_BombKick,
	itemPanel_Punch,
	itemPanel_PowerGlove5,
	itemPanel_PowerGlove3,
	itemPanel_FullFire,
	itemPanel_Heart,
	itemPanel_Shield,
	itemPanel_AutomaticShield,
	itemPanel_LineBomb,
	itemPanel_BombPass,
	itemPanel_SoftBlockPass,
	itemPanel_BombChange,
	itemPanel_Rocket,

	itemPanel_FireBuff,
	itemPanel_SpeedBuff,
	itemPanel_BombBuff,
	itemPanel_InvincibleSuit,

	itemPanel_Skull,
	itemPanel_Countdown,

	itemPanel_PowerBomb,
	itemPanel_InfiniteBomb,
	itemPanel_DangerousBomb,
	itemPanel_RemoteControl,
	itemPanel_RemoteControl_Retro,
	itemPanel_PierceBomb,
	itemPanel_SharpBomb,
	itemPanel_RubberBomb,
	itemPanel_TracingBomb,
	itemPanel_MineBomb,
	itemPanel_DangerousMineBomb,
	itemPanel_HeavyBomb,
	itemPanel_ColorBomb,
	itemPanel_Dynamite,

	itemPanel_QuickBomb,
	itemPanel_SlowBomb,
	itemPanel_MixBomb,

	itemPanel_MAX,
	itemPanel_None
};

class itemPanel :public element_accessableBoard
{
public:
	itemPanel								();
	void					Update			();
	void					Draw			();

	void					setExistence	(bool f)override;
	bool					checkHitPlayers	();

	static bool				getCanSetValue	(int);
	static int				getStrength		(int);

private:
	bool					appearing = false;
	int						burnCnt = -99;
	static int				GH_burning[BURN_EFF_NUM];
};