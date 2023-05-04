#include <string>
#include "DxLib.h"
#include "../h/RGB.h"
#include "../h/configLoader.h"
#include "../h/configMgr_teams.h"
#include "../h/graphicMgr_common.h"

using namespace std;

configMgr_teams::configMgr_teams() :configMgr(PLAYER_MAX)
{
    BasisDY = 32;
    setSave(true, FILEPATH_TEAM);

    configLoader cl(saveFileName, stepMAX);

    for (int i = 0; i < stepMAX; i++)
    {
        menuData mData;
        mData.Max = Team_MAX;
        mData.guideStrSingle = { "Choose player's controller and team.", "プレイヤーの所属チームを決めよう"};
        mData.cursor = cl.getConfig(i);
        setData(i, mData);

        graphMgr[i].setPlayerID(i);
    }
    mallocMI(0);
}
configMgr_teams::~configMgr_teams()
{
}

void configMgr_teams::Draw()
{
    int szX = 96;
    int szY = 48;
    int px = 128;

    for (int i = 0; i < Team_MAX; i++)
    {
        if (i != Team_None)
        {
            RgbData rgb;
            GetDrawBright(&rgb.r, &rgb.g, &rgb.b);
            SetDrawBrightTeam(i);
            graphicMgr_common::GetInstance().DrawBox(px - 8, 16, px + 72, WINDOW_HEIGHT - 100);
            graphicMgr_common::GetInstance().DrawBox(px - 8, 16, px + 72, WINDOW_HEIGHT - 100); 
            SetDrawBright(rgb.r, rgb.g, rgb.b);
        }
        px += szX;
    }
    for (int i = 0; i < stepMAX; i++)
    {
        if (mI != nullptr)
        {
            int cursor = (i == step) ? mI->getCursor() : mVec[i].cursor;
            int dx = 128 + szX * cursor;
            int dy = BasisDY + szY * i;

            graphMgr[i].DrawIcon(dx + 9, dy);
            
            if (i == step)
            {
                input_key* kI = mI->getInputKey();
                if (kI != nullptr)
                {
                    DrawRotaGraph(dx - 8, dy + 16, 1.0, 0.0, GH_marrow[kI->getDirection() == Direction_Left], TRUE, FALSE);
                    DrawRotaGraph(dx + 72, dy + 16, 1.0, 0.0, GH_marrow[kI->getDirection() == Direction_Right], TRUE, TRUE);
                }
            }
        }
    }
    DrawDialogue();
}
void configMgr_teams::Finalize()
{
    for (int s = 0; s < stepMAX; s++)
    {
        playerCounter::GetInstance().setTeamID(s, mVec[s].cursor);
    }
}