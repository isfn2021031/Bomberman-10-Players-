#pragma once
#include "../h/buff_base.h"

enum
{
	diseasedPattern_RandomFire,
	diseasedPattern_RandomSpeed,		// ランダムスピード
	diseasedPattern_SlowFoot,			// 鈍足
	diseasedPattern_ShunFoot,			// 駿足
	diseasedPattern_ReverseDirection,	// 操作反転
	diseasedPattern_Peace,				// 平和病
	diseasedPattern_WeakBomb,			// ヘボボム
	diseasedPattern_Stun,				// ランダムスタン
	diseasedPattern_Diarrhea,			// 下痢
	diseasedPattern_RandomCoolTime,
	diseasedPattern_Ring,				
	diseasedPattern_MineRush,			// 地雷ラッシュ
	diseasedPattern_HeavyRush,
	diseasedPattern_DangerousRush,
	diseasedPattern_DynamiteRush,		// ダイナマイトラッシュ

	diseasedPattern_MAX,
	diseasedPattern_None
};
class buff_diseased :public buff_base
{
public:
	buff_diseased		(int fcMAX = 480);
	void		Draw	(int dx, int dy)override {}

private:
	int			oldBuffSpeed = 0;
	void		bf_main	()override;
};