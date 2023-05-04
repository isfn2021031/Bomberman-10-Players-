#include "DxLib.h"
#include "../h/configMgr_button.h"
#include "../h/configLoader.h"
#include "../h/RGB.h"

#define INFO_X                  (10)                // ���̕`��J�n���W
#define INFO_Y                  (10)
#define INFO_SPACE              (24)                // ��s������ɋ󂯂�X�y�[�X
#define INFO_NAME_X             (140)               // ���͖���`�悷��X���W
#define INFO_INPUT_X            (320)               // ���͏�Ԃ�`�悷��X���W

configMgr_button::configMgr_button(string padName) :configMgr(actButton_MAX)
{
    BasisDX = 64;
    kI_ptr = new input_key(getPadIdFromPadName(padName));

    // �q�����Ă��� Joypad �̑I�����j���[���K�v
    string fpath = "bin/config/" + padName + ".bin";
    configLoader cl(fpath, actButton_MAX);
    const langData str[actButton_MAX] =
    {
        {   "Bomb, Decide",                 "�{���ݒu / ����"    },
        {   "Punch, Backspace",             "�p���` / �߂�"    },
        {   "Shield, Rocket Charge",        "�V�[���h"        },
        {   "Kick Stop, Remote Control",    "�L�b�N�X�g�b�v"        },
        {   "Open Pause Menu",              "�|�[�Y���j���["  },
    };
    vector<langData> cursorStr;
    for (int i = 0; i < BUTTON_MAX; i++)
    {
        string str = "Button" + to_string(i + 1);
        cursorStr.push_back({ str, str });
    }
    
    setSave(true, fpath);

    for (int c = 0; c < actButton_MAX; c++)
    {
        menuData md_def;
        md_def.cursor = cl.getConfig(c);
        md_def.cursorStr = cursorStr;
        md_def.labelStr = str[c];
        md_def.Max = BUTTON_MAX;
        setData(c, md_def);
    }
    mallocMI(0);
}
configMgr_button::~configMgr_button()
{
    if (kI_ptr != nullptr)
    {
        delete kI_ptr;
        kI_ptr = nullptr;
    }
}

int configMgr_button::Update()
{
    if (kI_ptr != nullptr)
    {
        kI_ptr->Update();
    }
    int retval = 0;
    
    if (!buttonTest)
    {
        retval = configMgr::Update();
    }
    if (CheckHitKey(KEY_INPUT_T) > OldTKey)
    {
        buttonTest = !buttonTest;
    }
    OldTKey = CheckHitKey(KEY_INPUT_T);

    return retval;
}
void configMgr_button::Draw()
{
    if (buttonTest)
    {
        int dx = WINDOW_WIDTH * 0.70;
        int dy = 64;
        int height = BUTTON_MAX / 3;

        for (int b = 0; b < BUTTON_MAX; b++)
        {
            if (kI_ptr != nullptr)
            {
                unsigned int color = (kI_ptr->getButtonReal(b)) ? RGB_BRIGHT_ORANGE : RGB_BLACK;
                string str = "Button" + to_string(b + 1);

                DrawFormatString(dx, dy, color, str.c_str());
                dy += 32;

                if ((b + 1) % height == 0)
                {
                    dx += 128;
                    dy = 64;
                }
            }
        }
    }
    configMgr::Draw();
}