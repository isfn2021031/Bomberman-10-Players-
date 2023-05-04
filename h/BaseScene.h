#pragma once
#include "../h/Task.h"
#include "../h/ISceneChanger.h"
#include "../h/music.h"
#include "../h/effect_fade.h"
#include "../h/input_base.h"

// �V�[���̊��N���X�B
class BaseScene : public Task 
{
public:
    BaseScene                           (ISceneChanger* changer);
    virtual             ~BaseScene      ();
    virtual void        Initialize      () override;        // �������������I�[�o�[���C�h�B
    virtual void        Finalize        () override;        // �I���������I�[�o�[���C�h�B
    virtual void        Update          () override;        // �X�V�������I�[�o�[���C�h�B
    virtual void        Draw            () override;        // �`�揈�����I�[�o�[���C�h�B
    static input_base*  GetNetworkInput () { return input_ptr; }

protected:
    ISceneChanger*      mSceneChanger = nullptr;                  // �N���X���L���ɃV�[���؂�ւ���`����C���^�[�t�F�C�X
    static input_base*  input_ptr;
    static musicMgr     musicData;
    static int          Network_mode;
    static int          GH_load[4];
    bool                bgmStop = false;
    int                 loadIdx = 0;

private:
    effect_fade*        efptr_fade = nullptr;
    static int          GH_Background;
};