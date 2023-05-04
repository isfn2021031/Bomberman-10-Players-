#include "DxLib.h"
#include "../h/Scene_Option.h"
#include "../h/Scene_Game.h"
#include "../h/Scene_KeyConfig.h"
#include "../h/Scene_StageMaker.h"
#include "../h/Scene_Menu.h"
#include "../h/Scene_NetworkCTR.h"
#include "../h/SceneMgr.h"

SceneMgr::SceneMgr() :mNextScene(eScene_None) // 次のシーン管理変数
{
    mScene = (BaseScene*) new Scene_Menu(this);
}

// 初期化
void SceneMgr::Initialize() 
{
    mScene->Initialize();
}

// 終了処理
void SceneMgr::Finalize() 
{
    mScene->Finalize();
}

// 更新
void SceneMgr::Update() 
{
    // 次のシーンがセットされていたら
    if (mNextScene != eScene_None)
    {        
        // 現在のシーンの終了処理を実行
        mScene->Finalize();
        delete mScene;

        // シーンによって処理を分岐
        switch (mNextScene)
        {
        case eScene_Menu:       mScene = (BaseScene*) new Scene_Menu(this);         break;
        case eScene_Game:       mScene = (BaseScene*) new Scene_Game(this);         break;
        case eScene_KeyConfig:  mScene = (BaseScene*) new Scene_KeyConfig(this);    break;
        case eScene_StageMaker: mScene = (BaseScene*) new Scene_StageMaker(this);   break;
        case eScene_Option:     mScene = (BaseScene*) new Scene_Option(this);       break;
        case eScene_Network:    mScene = (BaseScene*) new Scene_NetworkCTR(this);   break;
        default:    break;
        }
        mNextScene = eScene_None;    //次のシーン情報をクリア
        mScene->Initialize();    //シーンを初期化
    }
    mScene->Update(); //シーンの更新
}

// 描画
void SceneMgr::Draw() 
{
    mScene->Draw(); //シーンの描画
}

// 引数 nextScene にシーンを変更する
void SceneMgr::ChangeScene(eScene NextScene) 
{
    mNextScene = NextScene;    //次のシーンをセットする
}