#include <string>
#include "DxLib.h"
#include "../h/configMgr_items.h"
#include "../h/RGB.h"

using namespace std;

int configMgr_items::GH_checkBox[2] = { -1 };
int configMgr_items::GH_posi = -1;
int configMgr_items::GH_cross = -1;

configMgr_items::configMgr_items(int max) :configMgr(max)
{
    BasisDX = 64;
    BasisDY = 16;
    Interval = CELL_SIZE * 1.1;
 
    mallocMI(0);// Ç±ÇÍè¡Ç∑Ç∆ó·äOèoÇÈÇÊ

    if (GH_posi == -1)
    {
        GH_posi = LoadGraph("textures/icon/posi.png");
    }
    if (GH_cross == -1)
    {
        GH_cross = LoadGraph("textures/icon/cross.png");
    }
    if (GH_checkBox[0] == -1)
    {
        LoadDivGraph("textures/icon/checkBox.png", 2, 2, 1, CELL_SIZE, CELL_SIZE, GH_checkBox);
    }
}
configMgr_items::~configMgr_items()
{

}

void configMgr_items::Draw()
{
    if (mI != nullptr)
    {
        int n = (stepMAX / 4);
        int boxSize = 180;

        for (int s = 0; s < stepMAX; s++)
        {
            RgbData rgb;
            GetDrawBright(&rgb.r, &rgb.g, &rgb.b);

            int cur_real = (s == step) ? mI->getCursor() : mVec[s].cursor;
            int ndx = BasisDX + (64 * mVec[s].Max) * (s / n);
            int ndy = BasisDY + Interval * (s % n);

            if (!mVec[s].cursor)
            {
                SetDrawBright(64, 64, 64);
            }
            DrawIcon(s, ndx, ndy);
            SetDrawBright(rgb.r, rgb.g, rgb.b);
            DrawGraph(ndx - 32, ndy, GH_checkBox[cur_real], TRUE);

            if (s == step)
            {
                DrawReverseGraph(ndx - 50, ndy, GH_marrow[1], TRUE, TRUE);
            }
        }
        mI->DrawLabelStr(WINDOW_WIDTH * 0.45, WINDOW_HEIGHT * 0.70, WINDOW_WIDTH * 0.95, WINDOW_HEIGHT * 0.80);
    }
    DrawDialogue();
}