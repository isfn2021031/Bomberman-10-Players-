#include "../h/configMgr_other.h"
#include "../h/music.h"
#include "../h/soundMgr.h"

const menuData configMgr_other::mDatalist[sCONFIG_MAX] =
{
    {   SOUND_STYLES,   0,  {"Sound Style",                 "�T�E���h�Z�b�g"},       {  {"Nintendo DS","Nintendo DS�n"},   {"Wii","Wii�n"}   }     },
    {   BGM_MAX,        0,  {"Battle BGM",                  "�o�g��BGM"},
        {
            {"Super Bomberman","�X�[�p�[�{���o�[�}��"},    {"Super Bomberman 2","�X�[�p�[�{���o�[�}���Q"}, {"Super Bomberman 3","�X�[�p�[�{���o�[�}���R"},   
            {"Classic (Battle Pack)","�N���b�V�b�N1"},     {"Classic (DS-2)","�N���b�V�b�N�iDS-2�j"},      {"Classic (DS)","�N���b�V�b�N�iDS�j"},
            {"Classic (Blast)","�N���b�V�b�N�iWii�j"},     {"Lost World","���X�g���[���h"},                {"Ghost Town","�S�[�X�g�^�E��"},                
            {"Miner Cave","�}�C�i�[�P�C�u" },              {"Techno Machines","�e�N�m�}�V�[��"},           {"ForestStage (Blast)","�͂���ρiWii�j"},      
            {"PowerStage (Blast)","�M���M���p���[�iWii�j"},{"Super Bomberman R","�X�[�p�[�{���o�[�}��R"},  {"Random","���܂���"},
        },
    },
    {   5,              1,  {"BGM Volume",                  "BGM�̃{�����[��"},      {  {"0 %%","0 %%"}, {"25 %%","25 %%"}, {"50 %%","50 %%"}, {"75 %%","75 %%"}, {"100 %%","100 %%"}} },
    {   5,              1,  {"SE Volume",                   "SE�̃{�����[��"},       {  {"0 %%","0 %%"}, {"25 %%","25 %%"}, {"50 %%","50 %%"}, {"75 %%","75 %%"}, {"100 %%","100 %%"}} },
    {   3,              1,  {"Fast Forward Magnification",  "������̔{��"},         {  {"OFF","������Ȃ�"}, {"2x speed","2�{��"}, {"3x speed","3�{��"}} },
    {   2,              1,  {"Hit Stop Effect",             "�q�b�g�X�g�b�v���o"},   {  {"OFF","�Ȃ�"}, {"ON","����"}, } },
    {   2,              0,  {"Screen Mode",                 "��ʂ����Ă�"},         {  {"Window", "�E�B���h�E���[�h"}, {"Full Screen", "�t���X�N���[�����[�h"}, {"A reboot is required for the settings to take effect.", "���ݒ�𔽉f����ɂ͍ċN�����K�v�ł��B"}}},
    {   2,              0,  {"Language",                    "���ꂹ���Ă�"},         {  {"Japanese", "���{��i�ɂق񂲁j"}, {"English", "�p��i�������j"}    }  },
};
configMgr_other::configMgr_other() :configMgr(sCONFIG_MAX)
{
    configLoader cl(FILEPATH_OTHER, sCONFIG_MAX);

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
    setSave(true, FILEPATH_OTHER);
}
configMgr_other::~configMgr_other()
{
    soundMgr::GetInstance().Load();
    language::clear();
}