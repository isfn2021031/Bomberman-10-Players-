#include "DxLib.h"
#include "../h/Scene_NetworkCTR.h"
#include "../h/configMgr_number.h"
#include "../h/input_networkClient.h"
#include "../h/input_networkServer.h"
#include "../h/playerCounter.h"
#include "../h/language.h"
#include "../h/RGB.h"

Scene_NetworkCTR::Scene_NetworkCTR(ISceneChanger* changer) : BaseScene(changer)
{
    mI_ptr = new menuCursorMgr(5, true);
    mI_ptr->setCursorStr(language::getInstance()->getStringVec(
        {
            {"Server Side",     "他のPCからの接続を待機"}, 
            {"Client Side",     "他のPCに接続"},
            {"OFF",             "切断"},
            {"Edit IP Address", "IPアドレス編集"},
            {"Back",            "戻る"},
        }));

    configLoader sCl(FILEPATH_OTHER, sCONFIG_MAX);
    string FileName = "sound/bgm/network/" + to_string(sCl.getConfig(sCONFIG_SoundStyle)) + ".mp3";
    musicData.LoadMusicToString(FileName.c_str());
}
Scene_NetworkCTR::~Scene_NetworkCTR()
{
    if (mI_ptr != nullptr)
    {
        delete mI_ptr;
    }
    if (config_ptr != nullptr)
    {
        delete config_ptr;
    }
}

void Scene_NetworkCTR::Update()
{
    BaseScene::Update();

    if (ctrlock)
    {
        if (CheckHitKey(KEY_INPUT_SPACE))
        {
            Network_mode = 2;
            ctrlock = false;
            if (input_ptr != nullptr)
            {
                delete input_ptr;
                input_ptr = nullptr;
            }
        }
    }
    else
    {
        if (config_ptr != nullptr)
        {
            int retval = config_ptr->Update();
            if (retval != 0)
            {
                delete config_ptr;
                config_ptr = nullptr;
            }
        }
        else if (mI_ptr != nullptr)
        {
            int rv = mI_ptr->Update();
            if (rv > 0)
            {
                Network_mode = mI_ptr->getCursor();

                if (input_ptr != nullptr)
                {
                    delete input_ptr;
                    input_ptr = nullptr;
                }
                switch (Network_mode)
                {
                case 0:
                    input_network::Initialize();
                    input_ptr = new input_networkServer;
                    playerCounter::GetInstance().setPlayerCtr(PLAYER_MAX - 1, ctr_Network);
                    break;

                case 1:
                    input_network::Initialize();
                    input_ptr = new input_networkClient;
                    ctrlock = true;
                    break;

                case 3:
                    config_ptr = new configMgr_number(IPV4_DIGITS * IPV4_SEPARATE);
                default:
                    Network_mode = 2;
                    input_network::Finalize();
                    break;
                }
            }
            else if (rv < 0)
            {
                mSceneChanger->ChangeScene(eScene_Menu);
            }
        }
    }
    if (input_ptr != nullptr)
    {
        input_ptr->Update();

        if (frameCnt % 4 == 0)
        {
            loadIdx = (++loadIdx) % 4;
        }
        frameCnt++;
    }
}
void Scene_NetworkCTR::Draw()
{
    BaseScene::Draw();

    int dx = WINDOW_WIDTH * 0.05;
    int dy = WINDOW_HEIGHT * 0.6;
    vector<string> str = language::getInstance()->getStringVec(
        {
            { "Server Side","データ受信 OK" }, 
            { "Client Side","データ送信中...（あなたはプレイヤー10です）\nスペースキーで解除" },
            { "", "" },
            { "", "" },
            { "", "" },
        });
    DrawFormatString(dx + 202, WINDOW_HEIGHT - 62, RGB_BLACK, str[Network_mode].c_str());
    DrawFormatString(dx + 200, WINDOW_HEIGHT - 64, RGB_MUSCAT, str[Network_mode].c_str());
    DrawGraph(dx, dy, GH_load[loadIdx], TRUE);

    if (input_ptr != nullptr)
    {
        input_ptr->Draw();
    }
    if (mI_ptr != nullptr)
    {
        mI_ptr->Draw(WINDOW_WIDTH * 0.7, WINDOW_HEIGHT * 0.25);
    }
    if (config_ptr != nullptr)
    {
        config_ptr->Draw();
    }
}