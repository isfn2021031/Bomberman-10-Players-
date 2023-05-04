#include <string>
#include "DxLib.h"
#include "../h/configMgr_items_panel.h"
#include "../h/graphicMgr_common.h"
#include "../h/soundMgr.h"

using namespace std;

bool configMgr_items_panel::FirstTime = true;
int configMgr_items_panel::GH_images[itemPanel_MAX] = { -1 };

configMgr_items_panel::configMgr_items_panel() :configMgr_items(itemPanel_MAX)
{
    setSave(true, FILEPATH_SW_ITEMPANELS);

    menuData md;
    md.Max = 2;
    md.cursorStr = { {"OFF","OFF"}, {"ON","ON"}, };
    load(md);
    mallocMI(0);// ��������Ɨ�O�o���

    if (FirstTime)
    {
        string path = "textures/cutscene/item_";
        GH_images[itemPanel_Shield] =           LoadGraph((path + "shield.png").c_str());
        GH_images[itemPanel_AutomaticShield] =  LoadGraph((path + "shield.png").c_str());
        GH_images[itemPanel_Rocket] =           LoadGraph((path + "rocket.png").c_str());
        GH_images[itemPanel_BombChange] =       LoadGraph((path + "bombChange.png").c_str());
        GH_images[itemPanel_Countdown] =        LoadGraph((path + "countdown.png").c_str());
        FirstTime = false;
    }
}
configMgr_items_panel::~configMgr_items_panel()
{
}

int configMgr_items_panel::Update()
{
    int cur_old = 0;
    
    if (mI != nullptr)
    {
        cur_old = mI->getCursor();
    }
    int step_old = step;
    int retval = configMgr_items::Update();

    if (step > step_old)
    {
        if (cur_old)
        {
            soundMgr::GetInstance().Play(soundMgr::ConvertFromItem(step_old));
        }
    }
    return retval;
}
void configMgr_items_panel::Draw()
{
    if (step >= 0 && step < itemPanel_MAX)
    {
        DrawRotaGraph(WINDOW_WIDTH * 0.75, WINDOW_HEIGHT / 2, 1.0, 0.0, GH_images[step], TRUE);
    }
    configMgr_items::Draw();
}
void configMgr_items_panel::DrawIcon(int id, int x, int y)
{
    graphicMgr_common::GetInstance().DrawItemIcon(id, x, y);
}

bool configMgr_items_panel::mallocMI(int _step)
{
    if (configMgr::mallocMI(_step))
    {
        vector<langData> itemStr(itemPanel_MAX);

        itemStr[itemPanel_FireUp] =                     { "Fire Up\nBlast distance goes up 1 block.",                                                           "�t�@�C���[�A�b�v\n�{���̔������P�}�X�����Ȃ�" };
        itemStr[itemPanel_BombUp] =                     { "Bomb Up\nBombs that can be set at once go up 1.",                                                    "�{���A�b�v\n�{����u���鐔���P�R������" };
        itemStr[itemPanel_SpeedUp] =                    { "Speed Up\nSpeeds up movement.",                                                                      "�X�s�[�h�A�b�v\n�ړ��X�s�[�h�������Ȃ�" };
        itemStr[itemPanel_FireDown] =                   { "Fire Down\nBlast distance goes down 1 block.",                                                       "�t�@�C���[�_�E��\n�{���̔������P�}�X�Z���Ȃ�" };
        itemStr[itemPanel_BombDown] =                   { "Bomb Down\nBombs that can be set at once go down 1.",                                                "�{���_�E��\n�{����u���鐔���P�R����" };
        itemStr[itemPanel_SpeedDown] =                  { "Speed Down\nSlows down movement.",                                                                   "�X�s�[�h�_�E��\n�ړ��X�s�[�h���������Ȃ�" };
        itemStr[itemPanel_FullFire] =                   { "Full Fire\nBlast power maxed out for all bombs.",                                                    "�t���t�@�C���[\n���ׂẴ{�����ő�Η͂ɂȂ�" };
        itemStr[itemPanel_Heart] =                      { "Heart\nIncreases life by 1.",                                                                        "�n�[�g\n�n�[�g���P������" };
        itemStr[itemPanel_BombKick] =                   { "Kick\nBombs can be kicked.",                                                                         "�{���L�b�N\n�{����̓����肵�ăL�b�N�ł���悤�ɂȂ�" };
        itemStr[itemPanel_Punch] =                      { "Punch\nPunch bombs acroos the room.",                                                                "�p���`\n�{����B�{�^���Ńp���`���ĂR�}�X��ւƂ΂���" };
        itemStr[itemPanel_PowerGlove5] =                { "Power Glove\nBombs can be thrown.",                                                                  "�p���[�O���[�u\n�����̃{���������グ�A�T�}�X��֓����邱�Ƃ��ł���悤�ɂȂ�" };
        itemStr[itemPanel_PowerGlove3] =                { "Power Glove(monochrome)\nBombs can be thrown.",                                                      "���g���O���[�u\n�����̃{���������グ�A�R�}�X��֓����邱�Ƃ��ł���悤�ɂȂ�" };
        itemStr[itemPanel_PierceBomb] =                 { "Pierce Bomb\nBlast will penetrate soft blocks.",                                                     "�ђʃ{��\n�{���̔������\�t�g�u���b�N���ђʂ���悤�ɂȂ�" };
        itemStr[itemPanel_SharpBomb] =                  { "Sharp Bomb\nBlast will penetrate terrains.",                                                         "�V���[�v�{��\n�{���̔������������Q�����ђʂ���悤�ɂȂ�" };
        itemStr[itemPanel_PowerBomb] =                  { "Power Bomb\nBlast power maxed out for first bomb.",                                                  "�p���[�{��\n�ŏ��ɒu���{���̉Η͂��ő�ɂȂ�" };
        itemStr[itemPanel_InfiniteBomb] =               { "Infinite Bomb\nThe power of the first bomb is infinite.",                                            "���{��\n�ŏ��ɒu���{���̉Η͂������˒��ɂȂ�" };
        itemStr[itemPanel_RubberBomb] =                 { "Rubber Bomb\nBombs will bounce off walls.",                                                          "�{�������{��\n�{�����ǂɂ�����Ɣ��˂���悤�ɂȂ�" };
        itemStr[itemPanel_DangerousBomb] =              { "Dangerous Bomb\nFirst bomb is a Dangerous Bomb.",                                                    "�f���W�����X�{��\n�ŏ��ɂ����{���̔������i�q��ɍL����悤�ɂȂ�" };
        itemStr[itemPanel_RemoteControl] =              { "Remote Control\nAllows the player to detonate bombs at any given time.",                             "�����R��\n�������ݒu�����{����R�{�^���łP�R�����Ԃɔ����ł���悤�ɂȂ�" };
        itemStr[itemPanel_RemoteControl_Retro] =        { "Retrospective Remote Control \nAllows the player to detonate all bombs at any given time.",          "���g�������R��\n�������ݒu�������ׂẴ{����R�{�^���ł܂Ƃ߂Ĕ����ł���悤�ɂȂ�" };
        itemStr[itemPanel_MineBomb] =                   { "Mine Bomb\nFirst bomb sinks into the ground.",                                                       "�n���{��\n�ŏ��ɂ����{�����n�ʂɖ��܂�悤�ɂȂ�B\n�v���C���[�⃂���X�^�[���߂Â��ƋN������" };
        itemStr[itemPanel_DangerousMineBomb] =          { "Dangerous Mine\nFirst Dangerous Bomb sinks into the ground.",                                        "�n���f���W�����X�{��\n�ŏ��ɂ����{�����n�ʂɖ��܂�f���W�����X�{���ɂȂ�" };
        itemStr[itemPanel_HeavyBomb] =                  { "Heavy Bomb\nThe bomb cannot be moved.",                                                              "1t�{��\n�ŏ��ɂ����{�����L�b�N��p���`�ň�ؓ������Ȃ��Ȃ�\n1t�{���̔����͂ǂ�ȑ���ł��ꌂ�œ|���邼" };
        itemStr[itemPanel_ColorBomb] =                  { "Color Bomb\nIt makes the player invincible to their own bomb blasts.",                               "�J���t���{��\n�������Ă��_���[�W���󂯂Ȃ��{�����g����悤�ɂȂ�" };
        itemStr[itemPanel_Dynamite] =                   { "Dynamite\nFirst bomb is Dynamite.",                                                                  "�_�C�i�}�C�g\n�ŏ��ɂ����{�����_�C�i�}�C�g�ɂȂ�\n�_�C�i�}�C�g�̔����͂ǂ�ȑ���ł��ꌂ�œ|���邼" };
        itemStr[itemPanel_QuickBomb] =                  { "Quick Bomb\nIt will explode faster than usual.",                                                     "�N�C�b�N�{��\n�{���̃N�[���^�C�����Z���Ȃ�" };
        itemStr[itemPanel_SlowBomb] =                   { "Slow Bomb\nIt will explode later than usual.",                                                       "�X���[�{��\n�{���̃N�[���^�C���������Ȃ�" };
        itemStr[itemPanel_MixBomb] =                    { "Mix Bomb\nThe bombs placed will be random.",                                                         "�~�b�N�X�{��\n�ݒu����{���̎�ނ����񃉃��_���ɂȂ�" };
        itemStr[itemPanel_FireBuff] =                   { "Fire Buff\nThe blast distance will increasefor a short period of time.",                             "�t�@�C���[�o�t\n�������̂������A�{���̔����������Ȃ蒷���Ȃ�" };
        itemStr[itemPanel_BombBuff] =                   { "Bomb Buff\nBomb capacity will increase for a short period of time.",                                 "�{���o�t\n�������̂������A�{����ݒu�ł��鐔�����Ȃ葝����" };
        itemStr[itemPanel_SpeedBuff] =                  { "Speed Buff\nThe speed will increase for a short period of time.",                                    "�X�s�[�h�o�t\n�������̂������A�ړ��X�s�[�h�����Ȃ葬���Ȃ�" };
        itemStr[itemPanel_InvincibleSuit] =             { "Invincible Suit\nIt makes the player invincible against blasts, for a short period of time.",        "�t�@�C���[�X�[�c\n�������̂������A�_���[�W�������Ȃ����G��ԂɂȂ�" };

        itemStr[itemPanel_Countdown] =                  { "Countdown\nA countdown starts, and once time runs out you're toast.",                                "�J�E���g�_�E��\n�J�E���g�_�E���X�^�[�g�I\n�J�E���g���Ȃ��Ȃ�O�Ƀ��C�o���ɂԂ����ĂȂ�����悤" };
        itemStr[itemPanel_Skull] =                      { "Skull\nCauses many different diseases.",                                                             "�h�N��\n���܂��܂ȏǏ�̕a�C�ɂ�����" };

        itemStr[itemPanel_Shield] =                     { "Shield\nShields against bomb blasts.",                                                               "�V�[���h\nL�{�^���������Ă���ԁA���ʂ���̔�����h�����Ƃ��ł��邼" };
        itemStr[itemPanel_AutomaticShield] =            { "Automatic Shield\nShields against bomb blasts.",                                                     "�I�[�g�V�[���h\n���ʂ���̔����������Ŗh���V�[���h���g����悤�ɂȂ�\n�����Ă���Ƃ��͖h���Ȃ����璍�ӂ����" };
        itemStr[itemPanel_LineBomb] =                   { "Line Bomb\nAll bombs can be placed at once.",                                                        "���C���{��\n�{�^����f�����Q�񂨂���\n�{�����������ɕ��ׂ���悤�ɂȂ�" };
        itemStr[itemPanel_BombPass] =                   { "Bomb Pass\nAllows the player to walk through bombs.",                                                "�{���ʉ�\n�{���̏�������悤�ɂȂ�" };
        itemStr[itemPanel_BombChange] =                 { "Bomb Change\nTransforms you into a bomb.",                                                           "�{���ւ�\nL�������ƁA�{����������ɕϐg�ł���悤�ɂȂ�" };
        itemStr[itemPanel_Rocket] =                     { "Rocket\n",                                                                                           "���P�b�g\nL�𒷉����Ń`���[�W���āA��W�����v�I\n�󒆂ɂ��邠�����̓_���[�W���󂯂Ȃ���"};

        mI->setLabelStr(language::getInstance()->getString(itemStr[_step]));
        return true;
    }
    return false;
}