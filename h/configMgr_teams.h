#pragma once
#include "../h/playerCounter.h"
#include "../h/configMgr.h"

// ���j���[��ʃN���X
class configMgr_teams : public configMgr 
{
public:
                            configMgr_teams     ();
    virtual                 ~configMgr_teams    ();
    void                    Draw                ();
    virtual void            Finalize            ()override;

private:
    graphicMgr_bomberman    graphMgr[PLAYER_MAX];
};