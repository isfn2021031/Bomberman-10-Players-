#include <fstream>
#include "DxLib.h"
#include "../h/Scene_Game.h"
#include "../h/Scene_Menu.h"
#include "../h/configLoader.h"
#include "../h/dangerZoneForecast.h"
#include "../h/myfunc.h"
#include "../h/debug.h"
#include "../h/soundMgr.h"
#include "../h/bombMgr.h"

bool    Scene_Game::ShuffleSW_edit = false;
bool    Scene_Game::ShuffleSW_random = false;

int     Scene_Game::GH_GO = -1;
int     Scene_Game::GH_READY = -1;
int     Scene_Game::GH_FINISH = -1;

Scene_Game::Scene_Game(ISceneChanger* changer) : BaseScene(changer)
{
    if (GH_GO == -1)
    {
        GH_GO =     LoadGraph("textures/GO.png");
        GH_READY =  LoadGraph("textures/READY.png");
        GH_FINISH = LoadGraph("textures/FINISH.png");
    }
    kIptr = new input_key(DX_INPUT_KEY_PAD1);
    mIptr_pause = new menuCursorMgr_inGame(3, kIptr);
    mIptr_pause->setCursorStr(language::getInstance()->getStringVec(
        { 
            { "Resume Game",             "ゲームをつづける"       }, 
            { "Start The Game Over",     "やりなおす"             }, 
            { "Back To Main Menu",       "ゲームをやめる"         },
        }));
    mIptr_final = new menuCursorMgr(3, true);
    mIptr_final->setCursorStr(language::getInstance()->getStringVec(
        { 
            { "Play The Next Game",      "つぎのバトルへ" }, 
            { "Play The Game Again",     "おなじステージでバトル" }, 
            { "Back To Main Menu",       "メインメニューへもどる" },
        }));

    start = false;
    efREADY.flag = true;
    efGO.flag = false;
    efFINISH.flag = false;
    efFINISH.exrate = 3.0;
    bgmStop = true;

    configLoader cl_other(FILEPATH_OTHER, sCONFIG_MAX);
    srand(time(NULL));
    int n = cl_other.getConfig(sCONFIG_BattleBGM);
    int n2 = (n >= BGM_Random) ? rand() % (BGM_MAX - 1) : n;
    musicData.LoadMusicToString("sound/bgm/battle/" + (to_string(n2) + ".mp3"));

    fastRate = cl_other.getConfig(sCONFIG_FastForward);

    fstream fin(FPATH_STG_PARAMETER, ios::binary | ios::in);
    int stgID = 0;
    int cStyle = 0;
    fin >> stgID >> cStyle;
    fin.close();

    if (stgID == Stage_User)
    {
        configLoader cl(FILEPATH_CONF, CONFIG_MAX);

        if (ShuffleSW_edit)
        {
            vector<string> pathVec = getFileNamesFromFolder(FPATH_STG_FOLDER, { "stg" });
            if (pathVec.size())
            {
                board_withGimmick::setEditFileName(pathVec[rand() % pathVec.size()]);
            }
            ShuffleSW_edit = false;
        }
    }
    pMainBoard = new board_main(stgID, cStyle);
    dangerZoneForecast::GetInstance().Initialize();
}
Scene_Game::~Scene_Game()
{
    if (kIptr != nullptr)
    {
        delete kIptr;
        kIptr = nullptr;
    }
    if (mIptr_pause != nullptr)
    {
        delete mIptr_pause;
        mIptr_pause = nullptr;
    }
    if (mIptr_final != nullptr)
    {
        delete mIptr_final;
        mIptr_final = nullptr;
    }
    delete pMainBoard;
    dangerZoneForecast::GetInstance().Finalize();
}

void Scene_Game::Initialize()
{
    BaseScene::Initialize();
    soundMgr::GetInstance().Play(SE_READY);
}
void Scene_Game::Update()
{
    BaseScene::Update();

    if (!mIptr_pause->getPause())
    {
        if (pMainBoard != nullptr)
        {
            pMainBoard->Update();

            if (start)
            {
                pMainBoard->UpdateFlow();

                if (CheckHitKey(KEY_INPUT_TAB) == 1)
                {
                    for (int i = 0; i < fastRate; i++)
                    {
                        pMainBoard->UpdateFlow();
                    }
                }
            }
        }
    }
    if (start)
    {
        bombMgr::UpdateAll();
        
        if (mIptr_pause != nullptr)
        {
            if (kIptr != nullptr)
            {
                kIptr->Update();
            }
            if (gameOver)
            {
                bool nowCheck = soundMgr::GetInstance().check(SE_GameSet);
                static bool oldCheck = true;

                if (nowCheck)
                {
                    if (efFINISH.flag)
                    {
                        efFINISH.transparency += 3;
                        
                        if (efFINISH.exrate <= 0.7)
                        {
                            efFINISH.exrate = 0.7;
                        }
                        else
                        {
                            efFINISH.exrate = efFINISH.exrate - 0.04;
                        }
                    }
                }
                else 
                {
                    if (nowCheck < oldCheck)
                    {
                        configLoader sCl(FILEPATH_OTHER, sCONFIG_MAX);
                        string FileName = "sound/bgm/result/" + to_string(sCl.getConfig(sCONFIG_SoundStyle)) + ".mp3";
                        musicData.LoadMusicToString(FileName.c_str());
                        bgmStop = false;
                    }
                    else
                    {
                        // 最後のメニュー
                        if (element_canMove::getResult())
                        {
                            if (finalMenu)
                            {
                                if (mIptr_final->Update() > 0)
                                {
                                    ShuffleSW_edit = false;

                                    switch (mIptr_final->getCursor())
                                    {
                                    case 0:
                                    {            
                                        fstream fout(FPATH_STG_PARAMETER, ios::binary | ios::out);
                                        fout << rand() % Stage_MAX << " " << rand() % CellStyle_MAX;
                                        fout.close();
                                        ShuffleSW_edit = true;
                                        mSceneChanger->ChangeScene(eScene_Game);
                                        return;
                                    }
                                    // シーンをゲームに変更
                                    case 1:
                                        mSceneChanger->ChangeScene(eScene_Game);    
                                        return;

                                    case 2:
                                        mSceneChanger->ChangeScene(eScene_Menu);    // シーンをメニューに変更
                                        return;

                                    default:
                                        break;
                                    }
                                }
                            }
                            else
                            {
                                if (kIptr != nullptr)
                                {
                                    if (kIptr->getButtonEdge(actButton_A))
                                    {
                                        finalMenu = true;
                                    }
                                }
                            }
                        }
                        else
                        {
                            if (kIptr != nullptr)
                            {
                                if (kIptr->getButtonEdge(actButton_A))
                                {
                                    efFINISH.flag = false;
                                    element_canMove::setResult();
                                }
                            }
                        }

                    }
                }
                oldCheck = nowCheck;
            }
            else
            {
                int mI_rv = mIptr_pause->Update();

                // ポーズ画面中の処理
                if (mIptr_pause->getPause())
                {
                    mIptr_pause->setRestInput(false);

                    if (mI_rv == 1)
                    {
                        mSceneChanger->ChangeScene(eScene_Game);
                        return;
                    }
                    else if (mI_rv == 2)
                    {
                        mSceneChanger->ChangeScene(eScene_Menu);//シーンをメニューに変更
                        return;
                    }
                }
                else
                {
                    if (pMainBoard != nullptr)
                    {
                        // ゲームオーバーへ移行
                        if (pMainBoard->getGameOver())
                        {
                            gameOver = true;
                            bgmStop = true;
                            efFINISH.flag = true;
                            mIptr_pause->setRestInput(true);
                            soundMgr::GetInstance().Play(SE_GameSet);
                        }
                    }
                }
            }
        }
    }
    else
    {
        if (!efGO.flag)
        {
            efREADY.transparency += 3;
            efREADY.exrate = (efREADY.exrate >= 1.0) ? 1.0 : efREADY.exrate + 0.01;

            if (!soundMgr::GetInstance().check(SE_READY))
            {
                efREADY.flag = false;
                efGO.flag = true;
                soundMgr::GetInstance().Play(SE_GO);
            }
        }
        else
        {
            efGO.transparency += 8;
            efGO.exrate = (efGO.exrate > 2.0) ? 2.0 : efGO.exrate + 0.08;

            if (!soundMgr::GetInstance().check(SE_GO))
            {
                bgmStop = false;
                start = true;
                efGO.flag = false;

                if (pMainBoard != nullptr)
                {
                    pMainBoard->StartTimer();
                }
            }
        }
    }
    dangerZoneForecast::GetInstance().Update();
}
void Scene_Game::Draw()
{
    if (pMainBoard != nullptr)
    {
        pMainBoard->Draw();

        if (efFINISH.flag)
        {
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, efFINISH.transparency);
            DrawRotaGraph(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, efFINISH.exrate, 0.0, GH_FINISH, TRUE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        }
        else
        {
            if (finalMenu)
            {
                mIptr_final->Draw(WINDOW_WIDTH * 0.45, WINDOW_HEIGHT * 0.25);
            }
        }
    }
    if (gameOver && !efFINISH.flag)
    {
        playerCounter::GetInstance().Draw();
    }
    else
    {
        if (efREADY.flag)
        {
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, efREADY.transparency);
            DrawRotaGraph(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, efREADY.exrate, 0.0, GH_READY, TRUE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        }
        else if (efGO.flag)
        {
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, efGO.transparency);
            DrawRotaGraph(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, efGO.exrate, 0.0, GH_GO, TRUE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        }
        if (mIptr_pause != nullptr)
        {
            mIptr_pause->Draw(WINDOW_WIDTH * 0.4, WINDOW_HEIGHT * 0.3);
        }
    }
    if (DebugMgr::GetInstance().getMode() == Debug_Elements)
    {
        dangerZoneForecast::GetInstance().DrawStatus();
    }
}