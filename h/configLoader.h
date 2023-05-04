#pragma once
#include <string>
#include <vector>

#define FILEPATH_PL				("bin/player.bin")
#define FILEPATH_TEAM			("bin/team.bin")
#define FILEPATH_CONF			("bin/config/rules.bin")
#define FILEPATH_OTHER			("bin/config/sound.bin")
#define FILEPATH_SW_ITEMPANELS	("bin/config/itemPanel.bin")
#define FILEPATH_SW_ENEMIES		("bin/config/enemy.bin")

#define SOUND_STYLES			(2)

enum
{
	CONFIG_Time,
	CONFIG_StartPosition,
	CONFIG_SuddenDeath,
	CONFIG_Revenge,
	CONFIG_Handicap,
	CONFIG_Heart,
	CONFIG_PowerUpsViewer,
	CONFIG_Visualization,
	CONFIG_TeamInvincibility,
	CONFIG_Enemies,
	CONFIG_ItemDropRate,

	CONFIG_MAX,
};
enum
{
	sCONFIG_SoundStyle,
	sCONFIG_BattleBGM,
	sCONFIG_VolumeBGM,
	sCONFIG_VolumeSE,
	sCONFIG_FastForward,
	sCONFIG_HitStop,
	sCONFIG_DisplayMode,
	sCONFIG_Language,
	
	sCONFIG_MAX,
};

using namespace std;

class configLoader
{
public:
	configLoader						(string FileName, int _size);
	~configLoader						()				{}

	//////////////////////////////////////////////////	getter	///////////////////////////////////////////////////////////////	
	bool					getActive	()				{ return active; }
	int						getConfig	(int _idx);
	static vector<int>		GetCanSetVec(string filepath, int max);

	//////////////////////////////////////////////////	setter	///////////////////////////////////////////////////////////////	

private:
	vector<int>		vec;
	bool			active = false;
	int				size = 0;
};