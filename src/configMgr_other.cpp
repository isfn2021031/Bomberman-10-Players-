#include "../h/configMgr_other.h"
#include "../h/music.h"
#include "../h/soundMgr.h"

const menuData configMgr_other::mDatalist[sCONFIG_MAX] =
{
    {   SOUND_STYLES,   0,  {"Sound Style",                 "サウンドセット"},       {  {"Nintendo DS","Nintendo DS系"},   {"Wii","Wii系"}   }     },
    {   BGM_MAX,        0,  {"Battle BGM",                  "バトルBGM"},
        {
            {"Super Bomberman","スーパーボンバーマン"},    {"Super Bomberman 2","スーパーボンバーマン２"}, {"Super Bomberman 3","スーパーボンバーマン３"},   
            {"Classic (Battle Pack)","クラッシック1"},     {"Classic (DS-2)","クラッシック（DS-2）"},      {"Classic (DS)","クラッシック（DS）"},
            {"Classic (Blast)","クラッシック（Wii）"},     {"Lost World","ロストワールド"},                {"Ghost Town","ゴーストタウン"},                
            {"Miner Cave","マイナーケイブ" },              {"Techno Machines","テクノマシーン"},           {"ForestStage (Blast)","はらっぱ（Wii）"},      
            {"PowerStage (Blast)","ギンギンパワー（Wii）"},{"Super Bomberman R","スーパーボンバーマンR"},  {"Random","おまかせ"},
        },
    },
    {   5,              1,  {"BGM Volume",                  "BGMのボリューム"},      {  {"0 %%","0 %%"}, {"25 %%","25 %%"}, {"50 %%","50 %%"}, {"75 %%","75 %%"}, {"100 %%","100 %%"}} },
    {   5,              1,  {"SE Volume",                   "SEのボリューム"},       {  {"0 %%","0 %%"}, {"25 %%","25 %%"}, {"50 %%","50 %%"}, {"75 %%","75 %%"}, {"100 %%","100 %%"}} },
    {   3,              1,  {"Fast Forward Magnification",  "早送りの倍率"},         {  {"OFF","早送りなし"}, {"2x speed","2倍速"}, {"3x speed","3倍速"}} },
    {   2,              1,  {"Hit Stop Effect",             "ヒットストップ演出"},   {  {"OFF","なし"}, {"ON","あり"}, } },
    {   2,              0,  {"Screen Mode",                 "画面せってい"},         {  {"Window", "ウィンドウモード"}, {"Full Screen", "フルスクリーンモード"}, {"A reboot is required for the settings to take effect.", "※設定を反映するには再起動が必要です。"}}},
    {   2,              0,  {"Language",                    "言語せってい"},         {  {"Japanese", "日本語（にほんご）"}, {"English", "英語（えいご）"}    }  },
};
configMgr_other::configMgr_other() :configMgr(sCONFIG_MAX)
{
    configLoader cl(FILEPATH_OTHER, sCONFIG_MAX);

    for (int c = 0; c < stepMAX; c++)
    {
        menuData md = mDatalist[c];
        int v = cl.getConfig(c);

        if (v >= 0)
        {
            md.cursor = v;
        }
        setData(c, md);
    }
    mallocMI(step);
    setSave(true, FILEPATH_OTHER);
}
configMgr_other::~configMgr_other()
{
    soundMgr::GetInstance().Load();
    language::clear();
}