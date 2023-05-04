#pragma once
#include "../h/BaseScene.h"
#include "../h/configMgr.h"

// �ݒ��ʃN���X
class Scene_Option : public BaseScene
{
public:
    Scene_Option                    (ISceneChanger* changer);
    virtual         ~Scene_Option   ();
    void            Update          () override;            // �X�V�������I�[�o�[���C�h�B
    void            Draw            () override;            // �`�揈�����I�[�o�[���C�h�B

private:
    enum
    {
        config_key,
        config_rules,
        config_itempanels,
        config_enemies,
        config_P1sName,
        config_P1sCostume,
        config_other,

        config_MAX
    };

    menuCursorMgr*  mI = nullptr;
    menuCursorMgr*  mI_P1 = nullptr;
    configMgr*      confMgr = nullptr;
    int             curBGM_old = 0;
    bool            nowConfig = false;

    bool            mallocConfMgr   (int);
};