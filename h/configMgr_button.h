#pragma once
#include "../h/configMgr.h"

class configMgr_button :public configMgr
{
public:
    configMgr_button                    (string padName);
    virtual         ~configMgr_button   ();
    int             Update              ()override;
    void            Draw                ()override;

private:
    input_key*      kI_ptr = nullptr;
    bool            buttonTest = false;
    int             OldTKey = 0;
};