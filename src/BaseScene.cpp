#include "DxLib.h"
#include "../h/BaseScene.h"
#include "../h/soundMgr.h"

input_base* BaseScene::input_ptr = nullptr;

musicMgr BaseScene::musicData;

int BaseScene::Network_mode = 2;
int BaseScene::GH_load[4] = { -1 };
int BaseScene::GH_Background = -1;

BaseScene::BaseScene(ISceneChanger* changer)
{
    mSceneChanger = changer;

    if (GH_load[0] == -1)
    {
        LoadDivGraph("textures/load.png", 4, 4, 1, 192, 256, GH_load);
    }
    if (GH_Background == -1)
    {
        GH_Background = LoadGraph("textures/back.png");
    }
}
BaseScene::~BaseScene()
{
}
void BaseScene::Initialize()
{
    int fc = 0;
    loadIdx = 0;
    
    while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
    {
        if (GetASyncLoadNum() <= 0)
        {
            break;
        }
        if (fc % 4 == 0)
        {
            loadIdx = (++loadIdx) % 4;
        }
        fc++;
        DrawGraph(WINDOW_WIDTH * 0.05, WINDOW_HEIGHT * 0.6, GH_load[loadIdx], TRUE);
    }
    efptr_fade = new effect_fade(true, 4);

    while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
    {
        if (efptr_fade->Update() != 0)
        {
            break;
        }
        Draw();
        efptr_fade->Draw();
    }
    if (efptr_fade != nullptr)
    {
        delete efptr_fade;
        efptr_fade = nullptr;
    }

    soundMgr::GetInstance().Initialize();
}
void BaseScene::Finalize()
{
    efptr_fade = new effect_fade(false, 4);
    while (efptr_fade->Update() == 0 && ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
    {
        Draw();
        efptr_fade->Draw();
    }
    if (efptr_fade != nullptr)
    {
        delete efptr_fade;
        efptr_fade = nullptr;
    }
}
void BaseScene::Update()
{
    musicData.Update(bgmStop);
    soundMgr::GetInstance().Update();

    if (CheckHitKey(KEY_INPUT_M) != 0)
    {
        mSceneChanger->ChangeScene(eScene_Menu);    // シーンをメニューに変更
    }
}
void BaseScene::Draw() 
{
    DrawRotaGraph(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 1.0, 0.0, GH_Background, TRUE);
}