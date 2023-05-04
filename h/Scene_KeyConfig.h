#pragma once
#include <vector>
#include <set>
#include "DxLib.h"
#include "../h/BaseScene.h"
#include "../h/configMgr_button.h"
#include "../h/dialogue.h"

using namespace std;

// �ݒ��ʃN���X
class Scene_KeyConfig : public BaseScene
{
public:
    Scene_KeyConfig                             (ISceneChanger* changer);
    virtual                     ~Scene_KeyConfig();
    void                        Update          () override;            // �X�V�������I�[�o�[���C�h�B
    void                        Draw            () override;            // �`�揈�����I�[�o�[���C�h�B

private:
    bool                        padSelect = true;
    bool                        notFound = false;
    dialogue*                   dialogue_ptr = nullptr;
    menuCursorMgr*              mI_ptr = nullptr;
    configMgr_button*           config_ptr = nullptr;
    set<string>                 padNames;

    void                        mallocMI        ();
};