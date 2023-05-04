#pragma once
#include "../h/BaseScene.h"
#include "../h/board_base.h"
#include "../h/configMgr.h"
#include "../h/effect_rotaGraph3.h"
#include "../h/handiSelect.h"
#include "../h/dialogue.h"

enum
{
    Stage_Classic,		    // ���Ȃ���
    Stage_OpenField,	    // �͂����
    Stage_Potholes,
    Stage_BombBowling,
    Stage_ItemsGrab,		// �A�C�e����������
    Stage_EarthenPipe,
    Stage_BeltZone,
    Stage_Beltconveyor2,
    Stage_User,

    Stage_MAX
};

// ���j���[��ʃN���X
class Scene_Menu : public BaseScene 
{
public:
    Scene_Menu                      (ISceneChanger* changer);
    ~Scene_Menu                     ();
    void                    Update  () override;                // �X�V�������I�[�o�[���C�h�B
    void                    Draw    () override;                // �`�揈�����I�[�o�[���C�h�B

private:
    enum
    {
        step_Mode,
        step_Stage,
        step_StageStyle,
        step_Wait,

        step_MAX
    };
    enum
    {
        modeSelect_Battle,
        modeSelect_PlayerSelect,
        modeSelect_TeamSelect,
        modeSelect_NetworkCTR,
        modeSelect_Edit,
        modeSelect_Config,
        modeSelect_Exit,

        modeSelect_MAX
    };
    
    dialogue*               dialogue_ptr = nullptr;
    menuCursorMgr*          mI = nullptr;
    configMgr*              conf_ptr = nullptr;
    handiSelect*            mHandi = nullptr;
    board_base*             previewBoard = nullptr;
    configLoader*           pCL = nullptr;
    effect_rotaGraph3*      eff_title = nullptr;

    enum
    {
        after_none,
        after_game,
        after_restart,
        after_exit,
    }afterProcess;
    int                     nowState = 0;
    static graphicParam     GH_logo;

    int                     step = step_Mode;
    int                     values[step_MAX] = { 0 };

    void                    mallocMI    (int);
    bool                    mallocConfig(int);
    void                    ExitDialogue();
    bool                    modeSelect  ();
};