#include "DxLib.h"
#include "../h/menuCursorMgr.h"
#include "../h/soundMgr.h"
#include "../h/graphicMgr_common.h"
#include "../h/Rgb.h"

int menuCursorMgr::GH_window = -1;
int menuCursorMgr::GH_mbox = -1;
int menuCursorMgr::GH_mCursor = -1;

menuCursorMgr::menuCursorMgr(int max, bool movLast)
{
    cursorMAX = max;
    this->movLast = movLast;
    cursorStr.clear(); 
    lock.resize(max);
    GH_accents.resize(max);

    for (int i = 0; i < max; i++)
    {
        lock[i] = false;
        GH_accents[i] = -1;
    }
    if (GH_window == -1)
    {
        GH_window = LoadGraph("textures/win.png");
        GH_mbox = LoadGraph("textures/mbox.png");
        GH_mCursor = LoadGraph("textures/mCursor.png");
    }
    kI = new input_key(DX_INPUT_KEY_PAD1);
}
menuCursorMgr::~menuCursorMgr()
{
    if (kI != nullptr)
    {
        if (dynamic_kI)
        {
            delete kI;
        }
        kI = nullptr;
    }
}

int  menuCursorMgr::Update()
{
    if (kI != nullptr && dynamic_kI)
    {
        kI->Update();
    }
    return output();
}
void menuCursorMgr::Draw(int px, int py)
{
    const int by = py + 50;

    for (int i = 0; i < cursorMAX; i++)
    {
        if (!cursorStr[i].empty())
        {
            int bright = lock[i] ? 64 : (i == cursor ? 255 : 128);
            SetDrawBright(bright, bright, bright);

            int dx = (i == cursor) ? px - 16 : px;
            int dy = by + BoxRange.y * i;
            DrawExtendGraph(dx, dy, dx + (BoxRange.x - 20), dy + BoxRange.y, GH_mbox, TRUE);

            if (GH_accents[i] != -1)
            {
                DrawGraph(dx + (BoxRange.x - 70), dy + 1, GH_accents[i], TRUE);
            }
            SetDrawBright(255, 255, 255);
            DrawFormatString(dx + 20, dy + 4, (i == cursor) ? RGB_RED : (lock[i] ? RGB_GRAY : GetColor(32, 64, 128)), cursorStr[i].c_str());

            if (i == cursor)
            {
                DrawGraph(dx - BoxRange.y, dy, GH_mCursor, TRUE);
            }
        }
    }
    if (!labelStr.empty())
    {
        DrawExtendGraph(px - 30, py - 15, px + BoxRange.x, py + 35, GH_window, TRUE);
        DrawFormatString(px, py, GetColor(255, 255, 255), labelStr.c_str());
    }
}

void menuCursorMgr::DrawLabelStr(int x1, int y1, int x2, int y2)
{
    if (!labelStr.empty())
    {
        graphicMgr_common::GetInstance().DrawBox(x1, y1, x2, y2);
        graphicMgr_common::GetInstance().DrawBox(x1, y1, x2, y2);

        DrawFormatString(x1 + 10, y1 + 6, GetColor(0, 0, 0), labelStr.c_str());
    }
}
void menuCursorMgr::DrawGuideStr(int x1, int y1, int x2, int y2)
{
    if (!guideStr.empty())
    {
        graphicMgr_common::GetInstance().DrawBox(x1, y1, x2, y2);
        graphicMgr_common::GetInstance().DrawBox(x1, y1, x2, y2);

        DrawFormatString(x1 + 10, y1 + 6, GetColor(0, 0, 0), guideStr[cursor].c_str());
    }
}
void menuCursorMgr::DrawGuideStrSingle(int x1, int y1, int x2, int y2)
{
    if (!guideStrSingle.empty())
    {
        graphicMgr_common::GetInstance().DrawBox(x1, y1, x2, y2);
        graphicMgr_common::GetInstance().DrawBox(x1, y1, x2, y2);

        DrawFormatString(x1 + 10, y1 + 6, GetColor(0, 0, 0), guideStrSingle.c_str());
    }
}

void menuCursorMgr::normalization()
{
    if (cursor >= cursorMAX)
    {
        cursor = 0;
    }
    if (cursor < 0)
    {
        cursor = cursorMAX - 1;
    }
}
void menuCursorMgr::createCursor()
{
    switch (kI->getDirectionEdge())
    {
    case Direction_Down:    cursor++; break;
    case Direction_Up:      cursor--; break;
    default:                     break;
    }
}
void menuCursorMgr::setKeyCode(int code)
{
    if (kI != nullptr)
    {
        if (dynamic_kI)
        {
            delete kI;
        }
        kI = nullptr;
    }
    kI = new input_key(code);
}
void menuCursorMgr::setDecideButtonID(int id) 
{
    kI->setButton(actButton_A, id);
}
void menuCursorMgr::setBackButtonID(int id) 
{
    kI->setButton(actButton_B, id);
}
void menuCursorMgr::setAccentGraph(int c, string FileName)
{
    GH_accents[c] = LoadGraph(FileName.c_str());
}

string menuCursorMgr::getNowCursorStr()
{
    if (cursor >= 0 && cursor < cursorMAX)
    {
        return cursorStr[cursor];
    }
    return "";
}
int  menuCursorMgr::getSign()
{
    int oldY = cursor;
    
    createCursor();
    normalization();

    if (oldY != cursor)
    {
        soundMgr::GetInstance().Play(SE_Select);
    }

    if (kI->getButtonEdge(actButton_B))
    {
        return -1;
    }
    if (kI->getButtonEdge(actButton_A))
    {
        if (lock.empty())
        {
            return 1;
        }
        else if (!lock[cursor])
        {
            return 1;
        }
    }
    return 0;
}
int  menuCursorMgr::output()
{
    if (!restInput)
    {
        int rv = getSign();

        switch (rv)
        {
        case -1:
            if (movLast)
            {
                int lastC = cursorMAX - 1;
                if (cursor != lastC)
                {
                    cursor = lastC;
                    rv = 0;
                }
            }
            soundMgr::GetInstance().Play(SE_BackSpace);
            break;

        case 1:
            soundMgr::GetInstance().Play(SE_Decide);
            break;

        default:
            break;
        }
        return rv;
    }
    return 0;
}