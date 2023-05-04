#include "DxLib.h"
#include "../h/configMgr_costume.h"
#include "../h/language.h"
#include "../h/playerCounter.h"
#include "../h/graphicMgr_common.h"

configMgr_costume::configMgr_costume(int _playerID) :configMgr(costumeParts_MAX)
{
    playerID = _playerID;
    profData = playerCounter::GetInstance().getProfile(playerID);

    for (int c = 0; c < costumeParts_MAX; c++)
    {
        menuData md;  
        switch (c)
        {
        case costumeParts_Head:
            md.cursor = profData.costumeIDs.head;
            break;

        case costumeParts_Body:
            md.cursor = profData.costumeIDs.body;
            break;

        case costumeParts_Foot:
            md.cursor = profData.costumeIDs.foot;
            break;

        default:
            break;
        }
        md.Max = GetCostumeMAX(c);
        setData(c, md);
    }
    graphMgr.setPlayerID(playerID);
    graphMgr.setState(pose_walk);
    graphMgr.setCostumeID(profData.costumeIDs);

    mallocMI(step);
    setSave(false);
    setDialogue(false);
}
configMgr_costume::~configMgr_costume()
{
}

int configMgr_costume::Update()
{
    int step_old = step;
    int retval = configMgr::Update();

    if (mI != nullptr)
    {
        if (step == step_old)
        {
            switch (step_old)
            {
            case costumeParts_Head:
                profData.costumeIDs.head = mI->getCursor();
                break;

            case costumeParts_Body:
                profData.costumeIDs.body = mI->getCursor();
                break;

            case costumeParts_Foot:
                profData.costumeIDs.foot = mI->getCursor();
                break;

            default:
                break;
            }
        }
    }
    switch (retval)
    {
    case -1:   
        step = stepMAX - 1; 
        mallocMI(step);
        break;

    case 1:   
        step = 0;   
        mallocMI(step);
        playerCounter::GetInstance().setProfile(playerID, profData);
        break;

    default:           
        break;
    }
    graphMgr.setCostumeID(profData.costumeIDs);
    return retval;
}

void configMgr_costume::Draw()
{
    Draw(100, WINDOW_HEIGHT * 0.35);
}
void configMgr_costume::Draw(int px, int py)
{
    graphicMgr_common::GetInstance().DrawBox(px - 32, py - 32, px + 32, py + 32);
    graphMgr.Draw(px, py);

    if (mI != nullptr)
    {
        input_key* kI = mI->getInputKey();
        if (kI != nullptr)
        {
            int npy = py + 20 * step - 16;
            DrawRotaGraph(px - 30, npy, 1.0, 0.0, GH_marrow[kI->getDirection() == Direction_Left],  TRUE, FALSE);
            DrawRotaGraph(px + 30, npy, 1.0, 0.0, GH_marrow[kI->getDirection() == Direction_Right], TRUE, TRUE);
        }
    }
}

bool configMgr_costume::mallocMI(int _step)
{
    if (configMgr::mallocMI(_step))
    {
        if (mI != nullptr)
        {
            if (playerID > 0)
            {
                mI->setKeyCode(input_key::getPadID(playerID));
            }
        }
        return true;
    }
    return false;
}
