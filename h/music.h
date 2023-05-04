#pragma once

#include <string>
#include <vector>
#include <map>

enum
{
    BGM_SuperBomberman_1,
    BGM_SuperBomberman_2,
    BGM_SuperBomberman_3,

    BGM_Classic_BattlePack,
    BGM_Classic_DS_1,
    BGM_Classic_DS_2,
    BGM_Classic_Blast,

    BGM_LostWorld,
    BGM_GhostTown,
    BGM_MinerCave,
    BGM_TechnoMachines,

    BGM_ForestStage,
    BGM_PowerStage,
    
    BGM_SuperBombermanR_Central,

    BGM_Random,
    BGM_MAX
};

using namespace std;

class musicMgr
{
public:
                        musicMgr();
    virtual             ~musicMgr() {};
    
    void                PlayBGM();
    void                Update(bool stop);
    void                StopBGM();
    void                LoadMusicToString(string _name);

    vector<string>      names_all;
    int                 RecordMusicNum = 0;

private:
    string              lastName;

    vector<string>      names_menu;
    vector<string>      names_theme;
 
    map<string, int>    SH_BGM;
    map<string, bool>   checked;

    int                 SH_nowBGM;
    int                 volume = 0;
};