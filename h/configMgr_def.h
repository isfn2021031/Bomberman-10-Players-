#pragma once
#include "../h/configMgr.h"

class configMgr_def :public configMgr
{
public:
    configMgr_def   ();
    
private:
    static const menuData mDatalist[CONFIG_MAX];
};