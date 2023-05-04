#include <string>
#include "DxLib.h"
#include "../h/enemy.h"
#include "../h/configMgr_items_enemy.h"

using namespace std;

configMgr_items_enemy::configMgr_items_enemy() :configMgr_items(Enemy_MAX)
{
    setSave(true, FILEPATH_SW_ENEMIES);
    menuData md;
    md.Max = 2;
    md.cursorStr = { {"OFF","OFF"}, {"ON","ON"}, };
    load(md);
    mallocMI(0);// ��������Ɨ�O�o���
}
configMgr_items_enemy::~configMgr_items_enemy()
{
}

void configMgr_items_enemy::DrawIcon(int id, int x, int y)
{
    enemy::DrawEnemy(x, y, id, 0);
}
bool configMgr_items_enemy::mallocMI(int _step)
{
    if (configMgr::mallocMI(_step))
    {
        vector<langData> enemyStr(Enemy_MAX);

        enemyStr[Enemy_Ballom] =    { "Ballom\nFloating around.",                                       "�o����\n�t���t���Y���Ă��邾���B" };
        enemyStr[Enemy_ONeal] =     { "ONeal\nChasing slowly.",                                         "�I�j�[��\n�������ǂ������Ă���B" };
        enemyStr[Enemy_Dahl] =      { "Dahl\nMove around quickly.",                                     "�_��\n�f���������܂��B" };
        enemyStr[Enemy_Minvo] =     { "Minvo\nMove around quickly.",                                    "�~���{�[\n�f���������܂��B" };
        enemyStr[Enemy_Doria] =     { "Doria\nIt moves slowly, but it passes through soft blocks.",     "�R���h���A\n�����͒x�����ǁA�\�t�g�u���b�N��ʉ߂��Ă���B" };
        enemyStr[Enemy_Ovape] =     { "Ovape\nIt passes soft blocks.",                                  "�I�o�s�[\n�\�t�g�u���b�N��ʉ߂��Ă���B" };
        enemyStr[Enemy_Pass] =      { "Pass\nIt chases Bomberman at high speed. Easy to drop hearts.",  "�p�[�X\n�����Œǂ������Ă���B�n�[�g���h���b�v���₷���B" };
        enemyStr[Enemy_Pontan] =    { "Pontan\n.The strongest enemy.",                                  "�|���^��\n�ŋ��L�����B" };

        mI->setLabelStr(language::getInstance()->getString(enemyStr[_step]));
        return true;
    }
    return false;
}