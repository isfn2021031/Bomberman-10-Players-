#pragma once
#include "../h/configMgr.h"

class configMgr_items :public configMgr
{
public:
    configMgr_items                                 (int max);
    virtual                 ~configMgr_items        ();
    virtual void            Draw                    () override;               // 描画処理をオーバーライド。

protected:
    virtual void            DrawIcon                (int id, int x, int y) = 0;

private:
    static int              GH_posi;
    static int              GH_cross;
    static int              GH_checkBox[2];
};