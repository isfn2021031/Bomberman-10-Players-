#include <fstream>
#include <set>
#include "DxLib.h"
#include "../h/music.h"
#include "../h/configLoader.h"

musicMgr::musicMgr()
{
}

void musicMgr::LoadMusicToString(string _name)
{
	if (!checked[_name])
	{
		//SetCreateSoundDataType(DX_SOUNDDATATYPE_FILE);
		checked[_name] = true;
		SH_BGM[_name] = LoadSoundMem(_name.c_str());
	}
	if (lastName != _name)
	{
		StopSoundMem(SH_nowBGM);
	}
	SH_nowBGM = SH_BGM[_name];
	lastName = _name;

	configLoader cl(FILEPATH_OTHER, sCONFIG_MAX);
	volume = (255 / 5) * cl.getConfig(sCONFIG_VolumeBGM);
}

void musicMgr::PlayBGM()
{
	ChangeVolumeSoundMem(volume, SH_nowBGM);
	PlaySoundMem(SH_nowBGM, DX_PLAYTYPE_LOOP);
}
void musicMgr::Update(bool stop)
{
	if (stop)
	{
		StopBGM();
	}
	else
	{
		if (CheckSoundMem(SH_nowBGM))
		{
			ChangeVolumeSoundMem(volume, SH_nowBGM);
		}
		else
		{
			PlayBGM();
		}
	}
}
void musicMgr::StopBGM()
{
	StopSoundMem(SH_nowBGM);
}