#pragma once
#include "../h/Task.h"
#include "../h/ISceneChanger.h"
#include "../h/music.h"
#include "../h/effect_fade.h"
#include "../h/input_base.h"

// シーンの基底クラス。
class BaseScene : public Task 
{
public:
    BaseScene                           (ISceneChanger* changer);
    virtual             ~BaseScene      ();
    virtual void        Initialize      () override;        // 初期化処理をオーバーライド。
    virtual void        Finalize        () override;        // 終了処理をオーバーライド。
    virtual void        Update          () override;        // 更新処理をオーバーライド。
    virtual void        Draw            () override;        // 描画処理をオーバーライド。
    static input_base*  GetNetworkInput () { return input_ptr; }

protected:
    ISceneChanger*      mSceneChanger = nullptr;                  // クラス所有元にシーン切り替えを伝えるインターフェイス
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