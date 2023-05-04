#include <string>
#include "DxLib.h"
#include "../h/soundMgr.h"
#include "../h/configLoader.h"
#include "../h/itemPanel.h"

using namespace std;

soundMgr::soundMgr()
{
	for (int i = 0; i < SE_MAX; i++)
	{
		handle[i] = -1;
	}
	Initialize();
	Load();
}
soundMgr::~soundMgr()
{
	for (int i = 0; i < SE_MAX; i++)
	{
		DeleteSoundMem(handle[i]);
	}
}

void soundMgr::Initialize()
{
	playCache.clear();
}
void soundMgr::Update()
{
	int i = 0;

	for (auto& cache : playCache)
	{
		if (cache.frameCnt > 0)
		{
			cache.frameCnt--;
			if (cache.frameCnt <= 0)
			{
				Play(cache.seIdx);
			}
		}	
	}
	if (hitStopCnt)
	{
		hitStopCnt--;
		WaitTimer(40);
	}
}
void soundMgr::Load()
{
	SetCreateSoundDataType(DX_SOUNDDATATYPE_MEMNOPRESS);

	configLoader cl(FILEPATH_OTHER, sCONFIG_MAX);

	for (int i = 0; i < SE_MAX; i++)
	{
		bool variable = (i > SE_GameSet);

		string path1 = "sound/se/" + (variable ? "common" : to_string(cl.getConfig(sCONFIG_SoundStyle)));
		string path2 = "/" + to_string(i) + ".wav";

		if (handle[i] == -1 || variable)
		{
			DeleteGraph(handle[i]);
			handle[i] = LoadSoundMem((path1 + path2).c_str());
		}
	}
	volume = (255 / 5) * cl.getConfig(sCONFIG_VolumeSE);
}
void soundMgr::Play(int s, bool f)
{
	if (s >= 0 && s < SE_MAX)
	{
		if (f)
		{
			if (check(s))
			{
				return;
			}
		}
		ChangeVolumeSoundMem(volume, handle[s]);
		PlaySoundMem(handle[s], DX_PLAYTYPE_BACK);
	}
}
void soundMgr::setHitStop()
{
	hitStopCnt = 30;
	Play(SE_CriticalHit);
}

void soundMgr::CreateCache(SeCache cache)
{
	playCache.push_back(cache);
}
bool soundMgr::check(int s)
{
	return (CheckSoundMem(handle[s]) == 1);
}

int soundMgr::ConvertFromItem(int itemID)
{
	switch (itemID)
	{
	case itemPanel_Heart:			return SE_LifeUp;
	case itemPanel_FireUp:			return SE_FireUp;
	case itemPanel_SpeedUp:			return SE_SpeedUp;
	case itemPanel_BombUp:			return SE_BombUp;
	case itemPanel_FireDown:		return SE_FireDown;
	case itemPanel_SpeedDown:		return SE_SpeedDown;
	case itemPanel_BombDown:		return SE_BombDown;
	case itemPanel_BombKick:		return SE_CanKick;
	case itemPanel_BombPass:		return SE_BombPass;
	case itemPanel_Shield:			return SE_CanShield;
	case itemPanel_Rocket:			return SE_CanRocket;
	case itemPanel_LineBomb:		return SE_LineBomb;
	case itemPanel_RubberBomb:		return SE_RubberBomb;
	case itemPanel_RemoteControl:	return SE_RemoteControl;
	case itemPanel_HeavyBomb:		return SE_HeavyBomb;
	case itemPanel_ColorBomb:		return SE_ColorBomb;
	case itemPanel_QuickBomb:		return SE_QuickBomb;
	case itemPanel_SlowBomb:		return SE_SlowBomb;
	case itemPanel_MixBomb:			return SE_MixBomb;
	default:						break;
	}
	return -1;
}