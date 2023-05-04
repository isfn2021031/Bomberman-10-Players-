#pragma once
#include "../h/BaseScene.h"
#include "../h/menuCursorMgr.h"
#include "../h/configMgr.h"

class Scene_NetworkCTR : public BaseScene 
{
public:
    Scene_NetworkCTR                (ISceneChanger* changer);
    ~Scene_NetworkCTR               ();
    void                    Update  () override;             // 更新処理をオーバーライド。
    void                    Draw    () override;             // 描画処理をオーバーライド。

private:
    menuCursorMgr*          mI_ptr = nullptr;
    configMgr*              config_ptr = nullptr;
    bool                    ctrlock = false;
    int                     frameCnt = 0;
};