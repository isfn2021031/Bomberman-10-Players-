#include <cmath>
#include "DxLib.h"
#include "../h/configMgr_number.h"
#include "../h/graphicMgr_common.h"
#include "../h/menuCursorMgr_Y.h"

configMgr_number::configMgr_number(int _max) :configMgr(_max)
{
    setSave(true, FILEPATH_IPV4);

    configLoader cl(saveFileName, stepMAX);

    for (int i = 0; i < stepMAX; i++)
    {
        menuData mData;
        mData.Max = N_MAX;
        mData.cursor = cl.getConfig(i);
        setData(i, mData);
    }
    mallocMI(step);
}

int configMgr_number::Update()
{
    int retval = configMgr::Update();
    for (int i = 0; i < IPV4_SEPARATE; i++)
    {
        IP_address[i] = 0;
    }
    for (int i = 0; i < stepMAX; i++)
    {
        // ‚±‚Ì“ñ€‰‰ŽZ‚Í 0 ‚ÅŠ„‚é‚±‚Æ‚É‚æ‚é—áŠO‰ñ”ð
        int idx = i ? i / IPV4_DIGITS : 0;
        int mod = i ? i % IPV4_DIGITS : 0;
        int n = ((IPV4_DIGITS - 1) - mod);
        IP_address[idx] += pow(10.0, n) * mVec[i].cursor;
    }
    return retval;
}
void configMgr_number::Draw()
{
    int w = 400;
    for (int i = 0; i < IPV4_SEPARATE; i++)
    {
        int x1 = BasisDX - Interval + i * 128;
        int y1 = WINDOW_HEIGHT * 0.3;
        graphicMgr_common::GetInstance().DrawValue(x1, y1 + 2, 1.0, IP_address[i], 3);
        graphicMgr_common::GetInstance().DrawBox(x1 - 2, y1, x1 + 128, y1 + 40);
    }
    if (mI != nullptr)
    {
        for (int s = 0; s < stepMAX; s++)
        {
            int ndx = BasisDX + Interval * s;
            DrawFormatString(ndx, BasisDY, GetColor(0, 0, 0), to_string((s == step) ? mI->getCursor() : mVec[s].cursor).c_str());
        }
        mI->DrawGuideStrSingle(WINDOW_WIDTH * 0.2, WINDOW_HEIGHT * 0.2, WINDOW_WIDTH * 0.8, WINDOW_HEIGHT * 0.25);
    }
}

bool configMgr_number::mallocMI(int _step)
{
    if (_step >= 0 && _step < stepMAX)
    {
        if (mI != nullptr)
        {
            delete mI;
            mI = nullptr;
        }
        mI = new menuCursorMgr_Y(mVec[_step].Max);
        mI->setCursor(mVec[_step].cursor);
        mI->setLabelStr(language::getInstance()->getString(mVec[_step].labelStr));
        mI->setGuideStrSingle(language::getInstance()->getString(mVec[_step].guideStrSingle));
        return true;
    }
    return false;
}