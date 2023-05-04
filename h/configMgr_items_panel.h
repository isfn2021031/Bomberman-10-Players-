#pragma once
#include "../h/configMgr_items.h"
#include "../h/itemPanel.h"

class configMgr_items_panel :public configMgr_items
{
public:
    configMgr_items_panel                       ();
    virtual         ~configMgr_items_panel      ();

    virtual int     Update                      ()override;
    virtual void    Draw                        ()override;

    bool            mallocMI                    (int _step)override;

protected:
    virtual void    DrawIcon                    (int id, int x, int y)override;

private:
    static bool     FirstTime;
    static int      GH_images[itemPanel_MAX];
};