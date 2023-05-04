#pragma once
#include <vector>
#include "../h/Singleton.h"

using namespace std;

enum
{
	SE_READY,
	SE_GO,
	SE_GameSet,

	SE_Select,
	SE_Decide,
	SE_BackSpace,
	SE_PlusItem,
	SE_MinusItem,

	SE_LifeUp,
	SE_LifeDown,
	SE_FireUp,
	SE_SpeedUp,
	SE_BombUp,

	SE_FireDown,
	SE_SpeedDown,
	SE_BombDown,

	SE_CanKick,
	SE_BombPass,

	SE_CanShield,
	SE_CanRocket,

	SE_LineBomb,

	SE_RubberBomb,
	SE_RemoteControl,
	SE_HeavyBomb,
	SE_ColorBomb,
	SE_QuickBomb,
	SE_SlowBomb,
	SE_MixBomb,

	SE_BombSet,

	SE_Explosion_low,
	SE_Explosion_high,
	SE_Explosion_penetrate,
	SE_Explosion_dangerous,
	SE_Explosion_color,
	SE_Explosion_dynamite,

	SE_BombKick,
	SE_BombStop,
	SE_PowerGlove_1,
	SE_PowerGlove_2,
	SE_RocketJump,
	SE_Aura,
	SE_HurryUp,
	SE_Death,

	SE_CountDown,
	SE_Alarm,
	SE_Touch,
	SE_Reverse,
	SE_Warp,
	SE_PressureBlock,
	SE_Pause,
	SE_Chick,
	SE_Enemy,
	SE_FullCharge,
	SE_Respawn,
	SE_CriticalHit,

	SE_MAX,
};

struct SeCache
{
	int seIdx = 0;
	int frameCnt = 0;
};
class soundMgr :public Singleton<soundMgr>
{
public:
	friend class	Singleton<soundMgr>;	// Singletonでのインスタンス生成は許可
	void			Initialize		();
	void			Update			();
	void			Load			();
	void			Play			(int s, bool f = false);
	void			setHitStop		();

	void			CreateCache		(SeCache cache);
	bool			check			(int s);
	static int		ConvertFromItem	(int itemID);

private:
	soundMgr						();
	virtual			~soundMgr		();

	int				volume = 0;
	int				handle[SE_MAX] = { -1 };
	int				hitStopCnt = 0;
	vector<SeCache>	playCache;
};