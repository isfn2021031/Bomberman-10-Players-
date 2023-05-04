#include <fstream>
#include "DxLib.h"
#include "../h/configMgr.h"
#include "../h/RGB.h"
#include "../h/dialogue_withMenu.h"
#include "../h/graphicMgr_common.h"

int configMgr::GH_marrow[2] = { -1 };

configMgr::configMgr(int _max)
{
    stepMAX = _max;
    mVec.resize(stepMAX);
    unused = new bool[_max];

    for (int i = 0; i < _max; i++)
    {
        unused[i] = false;
    }
    mallocMI(0);

    if (GH_marrow[0] == -1)
    {
        LoadDivGraph("textures/marrow.png", 2, 1, 2, 32, 32, GH_marrow);
    }
}
configMgr::~configMgr()
{
    // 設定をセーブ（ファイル出力）
    if (save)
    {
        fstream fout;
        fout.open(saveFileName, ios::binary | ios::out);

        for (int y = 0; y < stepMAX; y++)
        {
            fout << mVec[y].cursor << " ";
        }
        fout.close();
    }
    if (mI != nullptr)
    {
        delete mI;
    }
    delete[] unused;
}

int configMgr::Update()
{
    if (dialogue_ptr != nullptr)
    {
        int retval = dialogue_ptr->Update();
        if (retval > 0)
        {
            save = false;
            return -1;
        }
        else if (retval < 0)
        {
            delete dialogue_ptr;
            dialogue_ptr = nullptr;
        }
    }
    else
    {
        int oldStep = step;
        if (mI != nullptr)
        {
            int rv = mI->Update();
            step += rv;
            if (unused[step])
            {
                step += (rv >= 0) ? 1 : -1;
            }
            if (step != oldStep)
            {
                if (oldStep >= 0 && oldStep < stepMAX)
                {
                    // カーソル位置を記録しておく
                    mVec[oldStep].cursor = mI->getCursor();
                }
                mallocMI(step);
            }
            if (step < 0)
            {
                step = 0;
             
                if (useDialogue)
                {
                    if (dialogue_ptr != nullptr)
                    {
                        delete dialogue_ptr;
                    }
                    dialogue_ptr = new dialogue_withMenu(language::getInstance()->getString(
                        { "The settings are not saved, but are you sure?", "設定は保存されませんがよろしいですか？" }));
                }
                else
                {
                    return -1;
                }
            }
            else if (step >= stepMAX || mI->getInputKey()->getButtonEdge(actButton_P))
            {
                if (step >= 0 && step < stepMAX)
                {
                    // カーソル位置を記録しておく
                    mVec[step].cursor = mI->getCursor();
                }
                return 1;
            }
        }
    }
    return 0;
}
void configMgr::Draw()
{
    int w = 400;
    int x1 = BasisDX - Interval;
    int y1 = BasisDY - Interval * 0.6;
    int x2 = BasisDX + w + Interval;
    int y2 = BasisDY + Interval * stepMAX;

    graphicMgr_common::GetInstance().DrawBox(x1, y1, x2, y2);
    graphicMgr_common::GetInstance().DrawBox(x1, y1, x2, y2);

    for (int s = 0; s < stepMAX; s++)
    {
        string str = language::getInstance()->getString(mVec[s].cursorStr[mVec[s].cursor]);
        int ndy = BasisDY + Interval * s;

        if (s == step)
        {
            str = language::getInstance()->getString(mVec[s].cursorStr[mI->getCursor()]);
            DrawBox(x1, ndy, x2, ndy + 20, GetColor(250, 200, 100), TRUE);
        }
        DrawFormatString(BasisDX, ndy, GetColor(0, 0, 0), language::getInstance()->getString(mVec[s].labelStr).c_str());
        DrawFormatString(BasisDX + w * 0.55, ndy, GetColor(0, 0, 0), str.c_str());
    }
    if (mI != nullptr)
    {
        mI->DrawGuideStrSingle(WINDOW_WIDTH * 0.2, WINDOW_HEIGHT * 0.75, WINDOW_WIDTH * 0.8, WINDOW_HEIGHT * 0.80);
    }
    
    DrawDialogue();
}
void configMgr::DrawDialogue()
{
    if (dialogue_ptr != nullptr)
    {
        dialogue_ptr->Draw();
    }
}

void configMgr::setStep(int _step) 
{
    step = _step; 
    mallocMI(_step);
}
void configMgr::setSave(bool flag, string _fileName)
{
    save = flag;
    saveFileName = _fileName;
}

bool configMgr::mallocMI(int _step)
{
    if (_step >= 0 && _step < stepMAX)
    {
        if (mI != nullptr)
        {
            delete mI;
        }
        mI = new menuCursorMgr_XdispSingle(mVec[_step].Max);
        mI->setCursor(mVec[_step].cursor);
        mI->setLabelStr(language::getInstance()->getString(mVec[_step].labelStr));
        mI->setCursorStr(language::getInstance()->getStringVec(mVec[_step].cursorStr));
        mI->setGuideStrSingle(language::getInstance()->getString(mVec[_step].guideStrSingle));
        return true;
    }
    return false;
}

void configMgr::load(menuData md)
{
    configLoader cl(saveFileName, stepMAX);

    for (int c = 0; c < stepMAX; c++)
    {
        int v = cl.getConfig(c);

        if (v >= 0)
        {
            md.cursor = v;
        }
        setData(c, md);
    }
}

int configMgr::getPadIdFromPadName(string padName)
{
    for (int i = 0; i < PAD_MAX; i++)
    {
        TCHAR joypadName[MAX_PATH];
        TCHAR productName[MAX_PATH];

        if (GetJoypadName(input_key::getPadID(i), joypadName, productName) == 0)
        {
            string joypadStr = joypadName;
            if (joypadStr == padName)
            {
                return input_key::getPadID(i);
            }
        }
    }
    return -1;
}
string configMgr::getPadNameFromPadId(int padId)
{
    TCHAR joypadName[MAX_PATH];
    TCHAR productName[MAX_PATH];

    if (GetJoypadName(padId, joypadName, productName) == 0)
    {
        string joypadStr = joypadName;
        return joypadStr;
    }
    return "";
}