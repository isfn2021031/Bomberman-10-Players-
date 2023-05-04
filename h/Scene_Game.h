#pragma once
#include "../h/BaseScene.h"
#include "../h/ISceneChanger.h"
#include "../h/menuCursorMgr_inGame.h"
#include "../h/board_main.h"

// ゲーム画面クラス
class Scene_Game : public BaseScene
{
public:
    Scene_Game                          (ISceneChanger* changer);
    ~Scene_Game                         ();
    void                    Initialize  () override;
    void                    Update      () override;            // 更新処理をオーバーライド
    void                    Draw        () override;            // 描画処理をオーバーライド

private:
    struct TextEffect
    {
        bool        flag = false;
        int         transparency = 0;
        double      exrate = 0.60;
    };
    TextEffect              efGO;
    TextEffect              efREADY;
    TextEffect              efFINISH;
    
    input_key*              kIptr = nullptr;
    menuCursorMgr_inGame*   mIptr_pause = nullptr;
    menuCursorMgr*          mIptr_final = nullptr;
    board_main*             pMainBoard = nullptr;

    bool                    start = false;
    bool                    gameOver = false;
    bool                    finalMenu = false;

    static bool             ShuffleSW_edit;
    static bool             ShuffleSW_random;

    int                     fastRate = 0;
    static int              GH_GO;
    static int              GH_READY;
    static int              GH_FINISH;
};