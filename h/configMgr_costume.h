#pragma once
#include "../h/configMgr.h"
#include "../h/graphicMgr_bomberman.h"

class configMgr_costume :public configMgr
{
public:
    configMgr_costume                               (int _playerID = 0);
    virtual                 ~configMgr_costume      ();
    int			            Update                  ()override;
    virtual void            Draw                    ()override;
    virtual void            Draw                    (int px, int py)override;

private:
    bool		            mallocMI                (int _step)override;
    
    userProfile             profData;
    graphicMgr_bomberman    graphMgr;
    int                     playerID = 0;
};