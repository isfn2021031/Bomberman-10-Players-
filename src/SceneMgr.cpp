#include "DxLib.h"
#include "../h/Scene_Option.h"
#include "../h/Scene_Game.h"
#include "../h/Scene_KeyConfig.h"
#include "../h/Scene_StageMaker.h"
#include "../h/Scene_Menu.h"
#include "../h/Scene_NetworkCTR.h"
#include "../h/SceneMgr.h"

SceneMgr::SceneMgr() :mNextScene(eScene_None) // ���̃V�[���Ǘ��ϐ�
{
    mScene = (BaseScene*) new Scene_Menu(this);
}

// ������
void SceneMgr::Initialize() 
{
    mScene->Initialize();
}

// �I������
void SceneMgr::Finalize() 
{
    mScene->Finalize();
}

// �X�V
void SceneMgr::Update() 
{
    // ���̃V�[�����Z�b�g����Ă�����
    if (mNextScene != eScene_None)
    {        
        // ���݂̃V�[���̏I�����������s
        mScene->Finalize();
        delete mScene;

        // �V�[���ɂ���ď����𕪊�
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
        mNextScene = eScene_None;    //���̃V�[�������N���A
        mScene->Initialize();    //�V�[����������
    }
    mScene->Update(); //�V�[���̍X�V
}

// �`��
void SceneMgr::Draw() 
{
    mScene->Draw(); //�V�[���̕`��
}

// ���� nextScene �ɃV�[����ύX����
void SceneMgr::ChangeScene(eScene NextScene) 
{
    mNextScene = NextScene;    //���̃V�[�����Z�b�g����
}