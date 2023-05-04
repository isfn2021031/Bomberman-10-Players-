#include <string>
#include <fstream>

#include "DxLib.h"
#include "../h/Scene_Menu.h"
#include "../h/board_preview.h"
#include "../h/configMgr_def.h"
#include "../h/configMgr_players.h"
#include "../h/configMgr_teams.h"
#include "../h/configMgr_items_panel.h"
#include "../h/configMgr_items_enemy.h"
#include "../h/dialogue_withMenu.h"
#include "../h/myfunc.h"
#include "../h/language.h"
#include "../h/playerCounter.h"

using namespace std;

graphicParam    Scene_Menu::GH_logo;

Scene_Menu::Scene_Menu(ISceneChanger* changer) : BaseScene(changer)
{
    pCL = new configLoader(FILEPATH_CONF, CONFIG_MAX);

    if (GH_logo.handle == -1)
    {
        GH_logo = LoadGraphParam("textures/logo.png");
    }
    eff_title = new effect_rotaGraph3(GH_logo, 0.005);

    configLoader sCl(FILEPATH_OTHER, sCONFIG_MAX);
    string FileName = "sound/bgm/menu/" + to_string(sCl.getConfig(sCONFIG_SoundStyle)) + ".mp3";
    musicData.LoadMusicToString(FileName.c_str());

    fstream fin(FPATH_STG_PARAMETER, ios::binary | ios::in);
    fin >> values[step_Stage] >> values[step_StageStyle];
    fin.close();

    mallocMI(0);    
    previewBoard = new board_preview(values[step_Stage], values[step_StageStyle]);

    playerCounter::GetInstance().ReSetup();
}
Scene_Menu::~Scene_Menu()
{
    if (conf_ptr != nullptr)
    {
        delete conf_ptr;
    }
    if (mHandi != nullptr)
    {
        delete mHandi;
    }
    if (mI != nullptr)
    {
        delete mI;
    }
    if (previewBoard != nullptr)
    {
        delete previewBoard;
    }
    if (pCL != nullptr)
    {
        delete pCL;
    }
    if (eff_title != nullptr)
    {
        delete eff_title;
    }
    if (dialogue_ptr != nullptr)
    {
        delete  dialogue_ptr;
    }
}

void Scene_Menu::Update()
{
    BaseScene::Update();

    static bool OldCustom = false;
    bool leaderCustom = playerCounter::GetInstance().Update();
    
    if (previewBoard != nullptr)
    {
        previewBoard->Update();
    }  
    if (eff_title != nullptr)
    {
        eff_title->Update();
    }

    if (!leaderCustom)
    {
        if (dialogue_ptr != nullptr)
        {
            int retval = dialogue_ptr->Update();
            if (retval != 0)
            {
                delete dialogue_ptr;
                dialogue_ptr = nullptr;

                switch (afterProcess)
                {
                case after_game:
                    if (retval > 0)
                    {
                        mSceneChanger->ChangeScene(eScene_Game);    // シーンをゲーム画面に変更
                    }
                    break;

                case after_restart:
                    step = step_Mode;
                    break;

                case after_exit:
                    if (retval > 0)
                    {
                        exit(-1);
                    }
                    return;

                default:
                    break;
                }
                afterProcess = after_none;
            }
        }
        else
        {
            int oldStep = step;

            switch (nowState)
            {
            case 0:
                step += mI->Update();

                if (step != oldStep)
                {
                    if (step > oldStep)
                    {
                        int old_value = values[oldStep];
                        values[oldStep] = mI->getCursor();

                        if (step >= step_Stage)
                        {
                            bool change = (old_value != values[oldStep]);
                            if (oldStep == step_Stage && values[step_Stage] == Stage_User)
                            {
                                change = true;
                                board_withGimmick::setEditFileName(getLoadFileName({ "stg file\0*.stg;\0","Load Stage" }));
                            }
                            if (change)
                            {
                                if (previewBoard != nullptr)
                                {
                                    delete previewBoard;
                                }
                                previewBoard = new board_preview(values[step_Stage], values[step_StageStyle]);
                            }
                        }
                        if (oldStep == step_Mode)
                        {
                            if (modeSelect())
                            {
                                return;
                            }
                        }
                        if (oldStep == step_Wait)
                        {
                            if (mallocConfig(mI->getCursor()))
                            {
                                return;
                            }
                            fstream fout(FPATH_STG_PARAMETER, ios::binary | ios::out);
                            fout << values[step_Stage] << " " << values[step_StageStyle];
                            fout.close();
                        }
                    }
                    mallocMI(step);
                }
                if (step < 0)
                {
                    ExitDialogue();
                    return;
                }
                if (step >= step_MAX)
                {
                    step = step_MAX - 1;
                    afterProcess = after_game;
                    if (dialogue_ptr != nullptr)
                    {
                        delete dialogue_ptr;
                        dialogue_ptr = nullptr;
                    }
                    dialogue_ptr = new dialogue_withMenu(language::getInstance()->getString({ "Are You Ready?", "ゲームをはじめますか？" }));
                }
                break;

            case 1:
            {
                int retval = conf_ptr->Update();

                if (retval != 0)
                {
                    if (retval > 0)
                    {
                        conf_ptr->Finalize();
                    }
                    delete conf_ptr;
                    conf_ptr = nullptr;
                    nowState = 0;
                    mallocMI(step);
                }
                break;
            }
            case 2:
                mHandi->Update();
                if (mHandi->checkFinished())
                {
                    nowState = 0;
                    delete mHandi;
                    mHandi = nullptr;
                    mallocMI(step);
                }
                break;

            default:
                break;
            }
        }
    }
    if (leaderCustom > OldCustom)
    {
        mallocMI(step);
    }
    OldCustom = leaderCustom;
}
void Scene_Menu::Draw()
{
    if (previewBoard != nullptr)
    {
        previewBoard->Draw();
    }
    switch (nowState)
    {
    case 0:
        if (mI != nullptr)
        {
            mI->Draw(WINDOW_WIDTH * 0.7, WINDOW_HEIGHT * 0.25);
            mI->DrawGuideStr(WINDOW_WIDTH * 0.20, WINDOW_HEIGHT * 0.70, WINDOW_WIDTH * 0.65, WINDOW_HEIGHT * 0.80);
        }
        if (eff_title != nullptr)
        {
            eff_title->Draw(WINDOW_WIDTH * 0.425, WINDOW_HEIGHT * 0.475);
        }
        break;

    case 1:
        if (conf_ptr != nullptr)
        {
            conf_ptr->Draw();
        }
        break;

    case 2:
        if (mHandi != nullptr)
        {
            mHandi->Draw();
        }
        break;

    default:
        break;
    }
    if (dialogue_ptr != nullptr)
    {
        dialogue_ptr->Draw();
    }
    playerCounter::GetInstance().Draw();
}

void Scene_Menu::mallocMI(int _step)
{
    if (_step >= 0 && _step < step_MAX)
    {
        if (mI != nullptr)
        {
            delete mI;
            mI = nullptr;
        }
        configLoader cl(FILEPATH_CONF, CONFIG_MAX);

        switch (_step)
        {
        case step_Mode:
            mI = new menuCursorMgr(modeSelect_MAX, true);
            mI->setCursorStr(language::getInstance()->getStringVec({ 
                {"Battle",          "バトルゲーム"},
                {"Player Select",   "プレイヤーセレクト"},
                {"Team Select",     "チームセレクト"},
                {"Network",         "ネットワーク"}, 
                {"Edit Stages",     "ステージ作り"}, 
                {"Option",          "オプション"},
                {"Exit",            "ゲーム終了"}
            }));
            mI->setLabelStr(language::getInstance()->getString({ "Select Mode!", "ゲームモードをえらんでね！" }));
            mI->setAccentGraph(modeSelect_TeamSelect, "textures/icon/flag.png");
            mI->setAccentGraph(modeSelect_Config, "textures/icon/conf.png");
            mI->setAccentGraph(modeSelect_Edit, "textures/icon/stg.png");
            break;

        case step_Stage:
            mI = new menuCursorMgr(Stage_MAX);
            mI->setCursorStr(language::getInstance()->getStringVec(
                { {"Normal","おなじみ"},{"Open Field","はらっぱ"},{"Potholes","あなぼこ"},{"Bomb Bowling","コロコロ"},
                {"Items Grab","アイテムザクザク"}, {"Pipe","ドカン"},{"Belt Zone","ベルトコンベア"},{"Belt Zone 2","ベルトコンベア２"}, {"User","作ったステージ"}}));
            mI->setLabelStr(language::getInstance()->getString({ "Select stage.", "ステージをえらんでね！" }));
            mI->setAccentGraph(Stage_User, "textures/icon/stg.png");
            mI->setAccentGraph(Stage_ItemsGrab, "textures/icon/treasureChest.png"); 
            mI->setGuideStr(language::getInstance()->getStringVec(
                {
                        {
                            "Simple stage.",
                            "おなじみのスタンダードなステージ。"
                        },
                        {
                            "You can run but you can't hide in this one!\n""No hard blocks to crouch behind!",
                            "ハードブロックがまったくないステージ。\nかくればしょがないので\nまわりにちゅういするんだ！"
                        },
                        {
                            "A stage folt of potholes!\nBomb blasts go right over them too,\nso be carefull.",
                            "あなぼこがいっぱいあいているステージ。\nばくふうはあなぼこをつうかするから\nきをつけるんだ！",
                        },
                        {
                            "Kicked bombs go in the direction of arrows,\nso kick your way to victory here!",
                            "キックしたボムがやじるしのほうこうにころがるステージ。\nやじるしをつかってキックこうげきだ！"
                        },
                        {
                            "A stash of items is visible from the start.\nGrab'em up fast and get the win!",
                            "アイテムがはじめからみえているステージ。\nいちばんはやくアイテムをあつめるのが\nしょうりへのちかみちだ！",
                        },
                        {
                            "You can hide in them, or set bombs in them!\nThere are pipes galore in the Pipes stage!",
                            "おおきなドカンがおかれたステージ。\nドカンのなかにばくだんをかくそう！",
                        },
                        {
                            "Your bombs get carried away automatically.",
                            "ベルトコンベアがおかれたステージ。\nながれてくるばくだんにきをつけるんだ！",
                        },
                        {
                            "Let's carry bombs on the belt",
                            "ベルトコンベアがおかれたステージ。\nながれてくるばくだんにきをつけるんだ！",
                        },
                        {
                            "Load the stage you created from a \".stg\" file.",
                            "「ステージ作り」で作ったステージを\nＳＴＧファイルからよみこもう！"
                        },
                }));
            break;

        case step_StageStyle:
            mI = new menuCursorMgr(CellStyle_MAX);
            mI->setCursorStr(language::getInstance()->getStringVec(
                { {"Classic","クラッシック"}, {"Retrospective","レトロ"}, {"Desert","さばく"}, {"Slip-Sliding","ツルツル"},
                {"Volcano","かざん"}, {"Darkness","くらやみ"}, {"Hyper Feet","いだてん"}, {"Factory", "こうじょう"}, {"PowerFreaks","ギンギンパワー"}}));
            mI->setLabelStr(language::getInstance()->getString({ "Select visual style.", "スタイルをえらんでね！" }));
            mI->setGuideStr(language::getInstance()->getStringVec(
                {
                    {
                        "No gimmicks.\nIt's easy to see who has true skills in this one.",
                        "いつもどおりののスタイル\nギミックがないので実力の差がわかりやすい！？"
                    },
                    {
                        "The blast distance is 2 from the start.",
                        "どこか懐かしいスタイル。\nデフォルトの火力も２マスに！",
                    },
                    {
                        "Set Mine Bombs with start.",
                        "砂漠のスタイル\nランダムに設置される地雷ボムに気をつけて！"
                    },
                    {
                        "Battle it out on a slippery ice floor.",
                        "凍ったじめんのうえでたたかう。\n移動したらすぐには止まれない！！",
                    },
                    {
                        "The blast distance is strong from the start.",
                        "最初からボム爆風の火力が高いデンジャラスなスタイル！\n周りをよく見て確実に避けよう"
                    },
                    {
                        "In the this style,\nThe stage is covered in darkness.",
                        "ステージ全体がまっくらに！\nいきどまりではさまれないように気をつけよう"
                    },
                    {
                        "You run at top speed in this stage.\nBut don't run too fast and lose focus!.",
                        "最高スピードでたたかうスタイル。\n注意して操作するんだ！",
                    },
                    {
                        "Items will be revived.",
                        "時間経過でアイテムが自然発生する\n長期戦にそなえよう！",
                    },
                    {
                        "No soft blocks.\nKicks & punches can be used,\nand blasts are strong from the start.",
                        "ソフトブロックとアイテムがなくなる。\nはじめから最強状態で\nキックやパンチもつかえるぞ！",
                    },
                }));
            mI->setAccentGraph(CellStyle_Speedy,        "textures/icon/speedup.png");
            mI->setAccentGraph(CellStyle_Volcano,       "textures/icon/fireup.png");
            mI->setAccentGraph(CellStyle_SlipSliding,   "textures/icon/snowCrystal.png");
            mI->setAccentGraph(CellStyle_Factory,       "textures/icon/conf.png");
            break;

        case step_Wait:
            mI = new menuCursorMgr(5);
            mI->setLock(
                {
                    false,
                    false,
                    (cl.getConfig(CONFIG_ItemDropRate) == 0),
                    (cl.getConfig(CONFIG_Enemies) == 0),
                    (cl.getConfig(CONFIG_Handicap) == 0),
                }
            );
            mI->setCursorStr(language::getInstance()->getStringVec(
                { 
                    {"Start",           "スタート"}, 
                    {"Settings",        "セットアップ"},
                    {"Item Panels",     "アイテムパネル"},
                    {"Enemies",         "敵キャラクター"},
                    {"Handi",           "ハンデ"} 
                }
            ));
            mI->setLabelStr(language::getInstance()->getString({ "Final Check.", "かくにん" }));
            mI->setGuideStr(language::getInstance()->getStringVec(
                { 
                    {"Start the batte!",                                "バトルスタート！"},        
                    {"Choose the rules of the battle.",                 "対戦ルールなどの設定ができるぞ"},
                    {"Decide which itempanels appear in the stage!",    "出現するアイテムパネルを設定できるぞ\n※設定するには「セットアップ」で\n「アイテムパネル」をなし以外にしよう"},
                    {"Decide which enemies appear in the stage!",       "出現する敵キャラクターを設定できるぞ\n※設定するには「セットアップ」で\n「敵キャラクター」を「あり」にしよう"},
                    {"Set various handicaps.",                          "各プレイヤーにハンデのせっていができるぞ\n※設定するには「セットアップ」で\n「ハンデ」を「あり」にしよう"}
                }
            ));
            break;

        default:
            return;
        }
        mI->setCursor(values[_step]);
    }
}
void Scene_Menu::ExitDialogue()
{
    step = 0;
    if (dialogue_ptr != nullptr)
    {
        delete dialogue_ptr;
        dialogue_ptr = nullptr;
    }
    afterProcess = after_exit;
    dialogue_ptr = new dialogue_withMenu(language::getInstance()->getString(
        { "Can You exit?", "ソフトを終了しますか？" }));
}

bool Scene_Menu::mallocConfig(int c)
{
    if (c == 0)
    {
        return false;
    }
    if (conf_ptr != nullptr)
    {
        delete conf_ptr;
    }
    step = step_Wait;
    nowState = 1;

    switch (c)
    {
    case 1:
        conf_ptr = new configMgr_def;
        break;

    case 2:
        conf_ptr = new configMgr_items_panel;
        break;

    case 3:
        conf_ptr = new configMgr_items_enemy;
        break;

    case 4:
        nowState = 2;
        mHandi = new handiSelect;

        if (dialogue_ptr != nullptr)
        {
            delete dialogue_ptr;
            dialogue_ptr = nullptr;
        }
        dialogue_ptr = new dialogue(language::getInstance()->getString(
            { "Set Ability.", "各プレイヤーに\n最初から持っているアイテムを設定しよう\n\nみそボンで復活したときにも効果が適用されるぞ！" }));
        break;

    default:
        return false;
    }
    return true;
}
bool Scene_Menu::modeSelect()
{
    switch (mI->getCursor())
    {
    case modeSelect_PlayerSelect:
        step = step_Mode;
        nowState = 1;

        if (conf_ptr != nullptr)
        {
            delete conf_ptr;
        }
        conf_ptr = new configMgr_players;
        return true;

    case modeSelect_TeamSelect:
        step = step_Mode;
        nowState = 1;

        if (conf_ptr != nullptr)
        {
            delete conf_ptr;
        }
        conf_ptr = new configMgr_teams;
        return true;

    case modeSelect_Config:
        mSceneChanger->ChangeScene(eScene_Option);
        return true;

    case modeSelect_Edit:
        mSceneChanger->ChangeScene(eScene_StageMaker);
        return true;

    case modeSelect_NetworkCTR:
        mSceneChanger->ChangeScene(eScene_Network);
        return true;

    case modeSelect_Exit:
        ExitDialogue();
        return true;

    default:
        break;
    }
    return false;
}