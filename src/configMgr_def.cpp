#include "../h/configMgr_def.h"
#include "../h/language.h"

const menuData configMgr_def::mDatalist[CONFIG_MAX] = 
{
    {   10, 1,  {	"Minutes",			    "タイム"	},              
        { {"1","１分"} ,{"2","２分"}, {"3","３分"}, {"4","４分"}, {"5","５分"}, {"6","６分"}, {"7","７分"}, {"8", "８分"}, {"9","９分"}, {"10","１０分"}},
        {"Set the amout of time per round.","戦う時間を１分単位で設定できるぞ"}
    },
    {   2,  0,  {	"Start Point",  		"開始位置"	        },      { {"Random","シャッフル"}, {"Fixed","固定"}},                                                   {"Choose where players start out.",                                     "バトル開始時のプレイヤーの位置を設定できるぞ"}},
    {   3,  1,  {	"Sudden Death",  	    "サドンデス"        },      { {"OFF","なし"} ,      {"Pressure Blocks","プレッシャーブロック"}, {"Pontan","ポンタン"}  },   {"Toggle Sudden Death ON or OFF.",                                      "サドンデスの種類を設定できるぞ"}},
    {   3,  0,  {	"Revenge",				"みそボン"	        },      { {"OFF","なし"} ,      {"ON","あり"},                              {"Super","スーパー"}},      {"Toggle Revenge ON or OFF.",                                           "みそボンの有無を決めよう　※「スーパー」にすると攻撃した相手のステータスで復活"}},
    {   2,  0,  {	"Handicaps",			"ハンデ"	        },      { {"OFF","なし"} ,      {"ON","あり"}},                                                         {"Toggle Handicaps ON or OFF.",                                         "ハンデの有無を決めよう"}},
    {   3,  0,  {	"Heart Number",			"ハートの数"	    },      { {"1","1"},            {"2","2"},                                  {"3","3"}},                 {"Choose heart Number.",                                                "バトル開始時のハートの数を設定できるぞ"}},
    {   2,  0,  {	"PowerUps Viewer",	    "パワーアップ表示"	},      { {"OFF","なし"} ,      {"ON","あり"}},                                                         {"Toggle PowerUps Viewer ON or OFF.",                                   "画面端にプレイヤーのステータスを表示するか設定ができるぞ"}},
    {   2,  0,  {	"Visual Assistance",	"爆風予告"	        },      { {"OFF","なし"} ,      {"ON","あり"}},                                                         {"When turned ON, you can see blasts distance of bombs.",               "「あり」にすると、爆風が発生する場所に赤く予告が出るようになるぞ"}},
    {   2,  0,  {	"Team Invincibility",	"チームアタック"	},      { {"OFF","あり"} ,      {"ON","なし"}},                                                         {"When turned ON, blasts will not hit your teammates.",                 "「なし」にすると、味方の爆風でダメージをうけなくなるぞ"}},
    {   2,  0,  {	"Enemies",	            "敵キャラクター"    },      { {"OFF","なし"} ,      {"ON","あり"}},                                                         {"Toggle enemies ON or OFF.",                                           "敵キャラの有無を決めよう"}},
    {   4,  2,  {	"Item Panels Capacity.","アイテムパネル"	},      { {"None","なし"}, {"Low","すくない"}, {"Medium","ふつう"}, {"High","おおい"}},                 {"Change the probability that items will appear from soft blocks.",     "アイテムパネルの出現率を設定できるぞ"}},
};

configMgr_def::configMgr_def() :configMgr(CONFIG_MAX)
{
    configLoader cl(FILEPATH_CONF, CONFIG_MAX);

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
    setSave(true, FILEPATH_CONF);
}