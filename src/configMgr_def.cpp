#include "../h/configMgr_def.h"
#include "../h/language.h"

const menuData configMgr_def::mDatalist[CONFIG_MAX] = 
{
    {   10, 1,  {	"Minutes",			    "�^�C��"	},              
        { {"1","�P��"} ,{"2","�Q��"}, {"3","�R��"}, {"4","�S��"}, {"5","�T��"}, {"6","�U��"}, {"7","�V��"}, {"8", "�W��"}, {"9","�X��"}, {"10","�P�O��"}},
        {"Set the amout of time per round.","�키���Ԃ��P���P�ʂŐݒ�ł��邼"}
    },
    {   2,  0,  {	"Start Point",  		"�J�n�ʒu"	        },      { {"Random","�V���b�t��"}, {"Fixed","�Œ�"}},                                                   {"Choose where players start out.",                                     "�o�g���J�n���̃v���C���[�̈ʒu��ݒ�ł��邼"}},
    {   3,  1,  {	"Sudden Death",  	    "�T�h���f�X"        },      { {"OFF","�Ȃ�"} ,      {"Pressure Blocks","�v���b�V���[�u���b�N"}, {"Pontan","�|���^��"}  },   {"Toggle Sudden Death ON or OFF.",                                      "�T�h���f�X�̎�ނ�ݒ�ł��邼"}},
    {   3,  0,  {	"Revenge",				"�݂��{��"	        },      { {"OFF","�Ȃ�"} ,      {"ON","����"},                              {"Super","�X�[�p�["}},      {"Toggle Revenge ON or OFF.",                                           "�݂��{���̗L�������߂悤�@���u�X�[�p�[�v�ɂ���ƍU����������̃X�e�[�^�X�ŕ���"}},
    {   2,  0,  {	"Handicaps",			"�n���f"	        },      { {"OFF","�Ȃ�"} ,      {"ON","����"}},                                                         {"Toggle Handicaps ON or OFF.",                                         "�n���f�̗L�������߂悤"}},
    {   3,  0,  {	"Heart Number",			"�n�[�g�̐�"	    },      { {"1","1"},            {"2","2"},                                  {"3","3"}},                 {"Choose heart Number.",                                                "�o�g���J�n���̃n�[�g�̐���ݒ�ł��邼"}},
    {   2,  0,  {	"PowerUps Viewer",	    "�p���[�A�b�v�\��"	},      { {"OFF","�Ȃ�"} ,      {"ON","����"}},                                                         {"Toggle PowerUps Viewer ON or OFF.",                                   "��ʒ[�Ƀv���C���[�̃X�e�[�^�X��\�����邩�ݒ肪�ł��邼"}},
    {   2,  0,  {	"Visual Assistance",	"�����\��"	        },      { {"OFF","�Ȃ�"} ,      {"ON","����"}},                                                         {"When turned ON, you can see blasts distance of bombs.",               "�u����v�ɂ���ƁA��������������ꏊ�ɐԂ��\�����o��悤�ɂȂ邼"}},
    {   2,  0,  {	"Team Invincibility",	"�`�[���A�^�b�N"	},      { {"OFF","����"} ,      {"ON","�Ȃ�"}},                                                         {"When turned ON, blasts will not hit your teammates.",                 "�u�Ȃ��v�ɂ���ƁA�����̔����Ń_���[�W�������Ȃ��Ȃ邼"}},
    {   2,  0,  {	"Enemies",	            "�G�L�����N�^�["    },      { {"OFF","�Ȃ�"} ,      {"ON","����"}},                                                         {"Toggle enemies ON or OFF.",                                           "�G�L�����̗L�������߂悤"}},
    {   4,  2,  {	"Item Panels Capacity.","�A�C�e���p�l��"	},      { {"None","�Ȃ�"}, {"Low","�����Ȃ�"}, {"Medium","�ӂ�"}, {"High","������"}},                 {"Change the probability that items will appear from soft blocks.",     "�A�C�e���p�l���̏o������ݒ�ł��邼"}},
};

configMgr_def::configMgr_def() :configMgr(CONFIG_MAX)
{
    configLoader cl(FILEPATH_CONF, CONFIG_MAX);

    for (int c = 0; c < stepMAX; c++)
    {
        menuData md = mDatalist[c];
        int v = cl.getConfig(c);

        if (v >= 0)
        {
            md.cursor = v;
        }
        setData(c, md);
    }
    mallocMI(step);
    setSave(true, FILEPATH_CONF);
}