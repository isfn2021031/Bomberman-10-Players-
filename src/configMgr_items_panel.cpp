#include <string>
#include "DxLib.h"
#include "../h/configMgr_items_panel.h"
#include "../h/graphicMgr_common.h"
#include "../h/soundMgr.h"

using namespace std;

bool configMgr_items_panel::FirstTime = true;
int configMgr_items_panel::GH_images[itemPanel_MAX] = { -1 };

configMgr_items_panel::configMgr_items_panel() :configMgr_items(itemPanel_MAX)
{
    setSave(true, FILEPATH_SW_ITEMPANELS);

    menuData md;
    md.Max = 2;
    md.cursorStr = { {"OFF","OFF"}, {"ON","ON"}, };
    load(md);
    mallocMI(0);// これ消すと例外出るよ

    if (FirstTime)
    {
        string path = "textures/cutscene/item_";
        GH_images[itemPanel_Shield] =           LoadGraph((path + "shield.png").c_str());
        GH_images[itemPanel_AutomaticShield] =  LoadGraph((path + "shield.png").c_str());
        GH_images[itemPanel_Rocket] =           LoadGraph((path + "rocket.png").c_str());
        GH_images[itemPanel_BombChange] =       LoadGraph((path + "bombChange.png").c_str());
        GH_images[itemPanel_Countdown] =        LoadGraph((path + "countdown.png").c_str());
        FirstTime = false;
    }
}
configMgr_items_panel::~configMgr_items_panel()
{
}

int configMgr_items_panel::Update()
{
    int cur_old = 0;
    
    if (mI != nullptr)
    {
        cur_old = mI->getCursor();
    }
    int step_old = step;
    int retval = configMgr_items::Update();

    if (step > step_old)
    {
        if (cur_old)
        {
            soundMgr::GetInstance().Play(soundMgr::ConvertFromItem(step_old));
        }
    }
    return retval;
}
void configMgr_items_panel::Draw()
{
    if (step >= 0 && step < itemPanel_MAX)
    {
        DrawRotaGraph(WINDOW_WIDTH * 0.75, WINDOW_HEIGHT / 2, 1.0, 0.0, GH_images[step], TRUE);
    }
    configMgr_items::Draw();
}
void configMgr_items_panel::DrawIcon(int id, int x, int y)
{
    graphicMgr_common::GetInstance().DrawItemIcon(id, x, y);
}

bool configMgr_items_panel::mallocMI(int _step)
{
    if (configMgr::mallocMI(_step))
    {
        vector<langData> itemStr(itemPanel_MAX);

        itemStr[itemPanel_FireUp] =                     { "Fire Up\nBlast distance goes up 1 block.",                                                           "ファイヤーアップ\nボムの爆風が１マス長くなる" };
        itemStr[itemPanel_BombUp] =                     { "Bomb Up\nBombs that can be set at once go up 1.",                                                    "ボムアップ\nボムを置ける数が１コ増える" };
        itemStr[itemPanel_SpeedUp] =                    { "Speed Up\nSpeeds up movement.",                                                                      "スピードアップ\n移動スピードが速くなる" };
        itemStr[itemPanel_FireDown] =                   { "Fire Down\nBlast distance goes down 1 block.",                                                       "ファイヤーダウン\nボムの爆風が１マス短くなる" };
        itemStr[itemPanel_BombDown] =                   { "Bomb Down\nBombs that can be set at once go down 1.",                                                "ボムダウン\nボムを置ける数が１コ減る" };
        itemStr[itemPanel_SpeedDown] =                  { "Speed Down\nSlows down movement.",                                                                   "スピードダウン\n移動スピードがおそくなる" };
        itemStr[itemPanel_FullFire] =                   { "Full Fire\nBlast power maxed out for all bombs.",                                                    "フルファイヤー\nすべてのボムが最大火力になる" };
        itemStr[itemPanel_Heart] =                      { "Heart\nIncreases life by 1.",                                                                        "ハート\nハートが１つ増える" };
        itemStr[itemPanel_BombKick] =                   { "Kick\nBombs can be kicked.",                                                                         "ボムキック\nボムを体当たりしてキックできるようになる" };
        itemStr[itemPanel_Punch] =                      { "Punch\nPunch bombs acroos the room.",                                                                "パンチ\nボムをBボタンでパンチして３マス先へとばせる" };
        itemStr[itemPanel_PowerGlove5] =                { "Power Glove\nBombs can be thrown.",                                                                  "パワーグローブ\n足元のボムを持ち上げ、５マス先へ投げることができるようになる" };
        itemStr[itemPanel_PowerGlove3] =                { "Power Glove(monochrome)\nBombs can be thrown.",                                                      "レトログローブ\n足元のボムを持ち上げ、３マス先へ投げることができるようになる" };
        itemStr[itemPanel_PierceBomb] =                 { "Pierce Bomb\nBlast will penetrate soft blocks.",                                                     "貫通ボム\nボムの爆風がソフトブロックを貫通するようになる" };
        itemStr[itemPanel_SharpBomb] =                  { "Sharp Bomb\nBlast will penetrate terrains.",                                                         "シャープボム\nボムの爆風があらゆる障害物を貫通するようになる" };
        itemStr[itemPanel_PowerBomb] =                  { "Power Bomb\nBlast power maxed out for first bomb.",                                                  "パワーボム\n最初に置くボムの火力が最大になる" };
        itemStr[itemPanel_InfiniteBomb] =               { "Infinite Bomb\nThe power of the first bomb is infinite.",                                            "∞ボム\n最初に置くボムの火力が無限射程になる" };
        itemStr[itemPanel_RubberBomb] =                 { "Rubber Bomb\nBombs will bounce off walls.",                                                          "ボヨヨンボム\nボムが壁にあたると反射するようになる" };
        itemStr[itemPanel_DangerousBomb] =              { "Dangerous Bomb\nFirst bomb is a Dangerous Bomb.",                                                    "デンジャラスボム\n最初におくボムの爆風が格子状に広がるようになる" };
        itemStr[itemPanel_RemoteControl] =              { "Remote Control\nAllows the player to detonate bombs at any given time.",                             "リモコン\n自分が設置したボムをRボタンで１コずつ順番に爆発できるようになる" };
        itemStr[itemPanel_RemoteControl_Retro] =        { "Retrospective Remote Control \nAllows the player to detonate all bombs at any given time.",          "レトロリモコン\n自分が設置したすべてのボムをRボタンでまとめて爆発できるようになる" };
        itemStr[itemPanel_MineBomb] =                   { "Mine Bomb\nFirst bomb sinks into the ground.",                                                       "地雷ボム\n最初におくボムが地面に埋まるようになる。\nプレイヤーやモンスターが近づくと起爆する" };
        itemStr[itemPanel_DangerousMineBomb] =          { "Dangerous Mine\nFirst Dangerous Bomb sinks into the ground.",                                        "地雷デンジャラスボム\n最初におくボムが地面に埋まるデンジャラスボムになる" };
        itemStr[itemPanel_HeavyBomb] =                  { "Heavy Bomb\nThe bomb cannot be moved.",                                                              "1tボム\n最初におくボムがキックやパンチで一切動かせなくなる\n1tボムの爆風はどんな相手でも一撃で倒せるぞ" };
        itemStr[itemPanel_ColorBomb] =                  { "Color Bomb\nIt makes the player invincible to their own bomb blasts.",                               "カラフルボム\n自爆してもダメージを受けないボムが使えるようになる" };
        itemStr[itemPanel_Dynamite] =                   { "Dynamite\nFirst bomb is Dynamite.",                                                                  "ダイナマイト\n最初におくボムがダイナマイトになる\nダイナマイトの爆風はどんな相手でも一撃で倒せるぞ" };
        itemStr[itemPanel_QuickBomb] =                  { "Quick Bomb\nIt will explode faster than usual.",                                                     "クイックボム\nボムのクールタイムが短くなる" };
        itemStr[itemPanel_SlowBomb] =                   { "Slow Bomb\nIt will explode later than usual.",                                                       "スローボム\nボムのクールタイムが長くなる" };
        itemStr[itemPanel_MixBomb] =                    { "Mix Bomb\nThe bombs placed will be random.",                                                         "ミックスボム\n設置するボムの種類が毎回ランダムになる" };
        itemStr[itemPanel_FireBuff] =                   { "Fire Buff\nThe blast distance will increasefor a short period of time.",                             "ファイヤーバフ\nすこしのあいだ、ボムの爆風ががかなり長くなる" };
        itemStr[itemPanel_BombBuff] =                   { "Bomb Buff\nBomb capacity will increase for a short period of time.",                                 "ボムバフ\nすこしのあいだ、ボムを設置できる数がかなり増える" };
        itemStr[itemPanel_SpeedBuff] =                  { "Speed Buff\nThe speed will increase for a short period of time.",                                    "スピードバフ\nすこしのあいだ、移動スピードがかなり速くなる" };
        itemStr[itemPanel_InvincibleSuit] =             { "Invincible Suit\nIt makes the player invincible against blasts, for a short period of time.",        "ファイヤースーツ\nすこしのあいだ、ダメージをうけない無敵状態になる" };

        itemStr[itemPanel_Countdown] =                  { "Countdown\nA countdown starts, and once time runs out you're toast.",                                "カウントダウン\nカウントダウンスタート！\nカウントがなくなる前にライバルにぶつかってなすりつけよう" };
        itemStr[itemPanel_Skull] =                      { "Skull\nCauses many different diseases.",                                                             "ドクロ\nさまざまな症状の病気にかかる" };

        itemStr[itemPanel_Shield] =                     { "Shield\nShields against bomb blasts.",                                                               "シールド\nLボタンを押している間、正面からの爆風を防ぐことができるぞ" };
        itemStr[itemPanel_AutomaticShield] =            { "Automatic Shield\nShields against bomb blasts.",                                                     "オートシールド\n正面からの爆風を自動で防ぐシールドが使えるようになる\n動いているときは防げないから注意するんだ" };
        itemStr[itemPanel_LineBomb] =                   { "Line Bomb\nAll bombs can be placed at once.",                                                        "ラインボム\nボタンを素早く２回おすと\nボムをいっきに並べられるようになる" };
        itemStr[itemPanel_BombPass] =                   { "Bomb Pass\nAllows the player to walk through bombs.",                                                "ボム通過\nボムの上を歩けるようになる" };
        itemStr[itemPanel_BombChange] =                 { "Bomb Change\nTransforms you into a bomb.",                                                           "ボムへんげ\nLを押すと、ボムそっくりに変身できるようになる" };
        itemStr[itemPanel_Rocket] =                     { "Rocket\n",                                                                                           "ロケット\nLを長押しでチャージして、大ジャンプ！\n空中にいるあいだはダメージを受けないぞ"};

        mI->setLabelStr(language::getInstance()->getString(itemStr[_step]));
        return true;
    }
    return false;
}