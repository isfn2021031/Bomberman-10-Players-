#include "DxLib.h"
#include "../h/Scene_Option.h"
#include "../h/configMgr_def.h"
#include "../h/configMgr_items_panel.h"
#include "../h/configMgr_items_enemy.h"
#include "../h/configMgr_other.h"
#include "../h/configMgr_costume.h"
#include "../h/menuCursorMgr_user.h"
#include "../h/playerCounter.h"

Scene_Option::Scene_Option(ISceneChanger* changer) : BaseScene(changer)
{
    mI = new menuCursorMgr(config_MAX);
    mI->setCursorStr(language::getInstance()->getStringVec({
        {"Key Config",      "ボタン配置"},
        {"Settings",        "セットアップ"}, 
        {"Item Panels",     "アイテムパネル"},
        {"Enemies",         "敵キャラクター"},
        {"Nickname Records","ネーム一覧"},
        {"P1's Costume",    "1Pのコスチューム"}, 
        {"Other Settings",  "その他"},
    }));
    mI->setGuideStr(language::getInstance()->getStringVec({
        {"Change Key Config.",                              "ボタンに割り当てる操作をカスタマイズしよう"},
        {"Choose the rules of the battle.",                 "対戦ルールなどの設定ができるぞ"},
        {"Decide which itempanels appear in the stage!",    "ステージにおかれるアイテムパネルをカスタマイズしよう"},
        {"Decide which enemies appear in the stage!",       "ステージに現れる敵キャラクターをカスタマイズしよう"},
        {"Change Player1's nickname.",                      "ネームごとに戦績を確認できるぞ"},
        {"Change Player1's costume.",                       "プレイヤー1のコスチュームを変更する"},
        {"Change the audio settings.",                      "サウンドや言語の設定ができる"}, 
    }));
}
Scene_Option::~Scene_Option()
{
    if (confMgr != nullptr)
    {
        delete confMgr;
    }
    if (mI != nullptr)
    {
        delete mI;
    }
}

void Scene_Option::Update()
{
    BaseScene::Update();

    if (mI != nullptr)
    {
        int sysCmd = mI->Update();

        if (!nowConfig)
        {
            if (sysCmd > 0)
            {
                mallocConfMgr(mI->getCursor());
                nowConfig = true;
                return;
            }
            if (sysCmd < 0)
            {
                mSceneChanger->ChangeScene(eScene_Menu);
                return;
            }
        }
        else
        {
            if (confMgr != nullptr)
            {
                if (confMgr->Update() != 0)
                {
                    nowConfig = false;
                    delete confMgr;
                    confMgr = nullptr;
                }
                else
                {
                    // BGMのプレビュー
                    if (mI->getCursor() == config_other)
                    {
                        if (confMgr->getStep() <= sCONFIG_BattleBGM)
                        {
                            menuCursorMgr* ptr = confMgr->getMI();
                            if (ptr != nullptr)
                            {
                                int nowCur = ptr->getCursor();
                                if (nowCur != curBGM_old)
                                {
                                    string fbase;
                                    switch (confMgr->getStep())
                                    {
                                    case sCONFIG_SoundStyle:    
                                        if (nowCur < SOUND_STYLES)
                                        {
                                            fbase = "menu/";
                                        }
                                        break;

                                    case sCONFIG_BattleBGM:
                                        if (nowCur != BGM_Random)
                                        {
                                            fbase = "battle/";
                                        }
                                        break;

                                    default:
                                        break;
                                    }
                                    if (!fbase.empty())
                                    {
                                        string FileName = "sound/bgm/" + fbase + to_string(nowCur) + ".mp3";
                                        musicData.LoadMusicToString(FileName.c_str());
                                    }
                                }
                                curBGM_old = nowCur;
                            }
                        }
                    }
                }
            }
            else if (mI_P1 != nullptr)
            {
                if (mI_P1->Update() != 0)
                {
                    nowConfig = false;
                    delete mI_P1;
                    mI_P1 = nullptr;
                }
            }
        }
        mI->setRestInput(nowConfig);
    }
}
void Scene_Option::Draw()
{
    BaseScene::Draw();      // 親クラスの描画メソッドを呼ぶ

    if (confMgr != nullptr)
    {
        confMgr->Draw();
    }
    else
    {
        if (mI != nullptr)
        {
            mI->Draw(WINDOW_WIDTH * 0.7, WINDOW_HEIGHT * 0.25);
            mI->DrawGuideStr(WINDOW_WIDTH * 0.50, WINDOW_HEIGHT * 0.750, WINDOW_WIDTH * 0.9, WINDOW_HEIGHT * 0.850);

            if (mI_P1 != nullptr)
            {
                mI_P1->Draw(WINDOW_WIDTH * 0.475, WINDOW_HEIGHT * 0.45);
                mI_P1->DrawGuideStr(32, 32, WINDOW_WIDTH * 0.475, 0);
            }
        }
    }
}
bool Scene_Option::mallocConfMgr(int n) 
{
    if (confMgr != nullptr)
    {
        delete confMgr;
        confMgr = nullptr;
    }
    switch (n)
    {
    case config_key:
        mSceneChanger->ChangeScene(eScene_KeyConfig);
        return false;

    case config_rules:
        confMgr = new configMgr_def;
        return true;

    case config_itempanels:
        confMgr = new configMgr_items_panel;
        return true;

    case config_enemies:
        confMgr = new configMgr_items_enemy;
        return true;

    case config_other: 
        confMgr = new configMgr_other;  
        return true;
    
    case config_P1sName:
        mI_P1 = new menuCursorMgr_user;
        break;
    
    case config_P1sCostume:
        confMgr = new configMgr_costume(0);
        break;
    
    default:   
        break;
    }
    return false;
}