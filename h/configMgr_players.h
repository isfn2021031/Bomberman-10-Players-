#pragma once
#include "../h/playerCounter.h"
#include "../h/menuCursorMgr_button.h"
#include "../h/configMgr.h"

// ���j���[��ʃN���X
class configMgr_players : public configMgr 
{
public:
                            configMgr_players   ();
    virtual                 ~configMgr_players  ();
    int                     Update              ();
    virtual void            Draw                ()override;

private:
};