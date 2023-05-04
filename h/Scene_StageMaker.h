#pragma once
#include "../h/BaseScene.h"
#include "../h/ISceneChanger.h"
#include "../h/input_key.h"
#include "../h/board_make.h"
#include "../h/menuCursorMgr_inGame.h"

class Scene_StageMaker : public BaseScene
{
public:
    Scene_StageMaker                (ISceneChanger* changer);
    ~Scene_StageMaker               ();
    void                    Update  () override;        //更新処理をオーバーライド。
    void                    Draw    () override;            //描画処理をオーバーライド。

private:
    input_key*              kIptr = nullptr;
    board_make*             mk_board = nullptr;
    menuCursorMgr_inGame*   mIptr = nullptr;
    int                     startPattern = StartPattern_Normal;
};