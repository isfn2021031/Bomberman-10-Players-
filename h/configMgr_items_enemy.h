#pragma once
#include "../h/configMgr_items.h"

class configMgr_items_enemy :public configMgr_items
{
public:
    configMgr_items_enemy                       ();
    virtual         ~configMgr_items_enemy      ();
    bool            mallocMI                    (int _step) override;

protected:
    virtual void    DrawIcon                    (int id, int x, int y)override;
};