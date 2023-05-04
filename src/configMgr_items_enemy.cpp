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
    mallocMI(0);// これ消すと例外出るよ
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

        enemyStr[Enemy_Ballom] =    { "Ballom\nFloating around.",                                       "バロム\nフワフワ漂っているだけ。" };
        enemyStr[Enemy_ONeal] =     { "ONeal\nChasing slowly.",                                         "オニール\nゆっくり追いかけてくる。" };
        enemyStr[Enemy_Dahl] =      { "Dahl\nMove around quickly.",                                     "ダル\n素早く動きまわる。" };
        enemyStr[Enemy_Minvo] =     { "Minvo\nMove around quickly.",                                    "ミンボー\n素早く動きまわる。" };
        enemyStr[Enemy_Doria] =     { "Doria\nIt moves slowly, but it passes through soft blocks.",     "コンドリア\n動きは遅いけど、ソフトブロックを通過してくる。" };
        enemyStr[Enemy_Ovape] =     { "Ovape\nIt passes soft blocks.",                                  "オバピー\nソフトブロックを通過してくる。" };
        enemyStr[Enemy_Pass] =      { "Pass\nIt chases Bomberman at high speed. Easy to drop hearts.",  "パース\n高速で追いかけてくる。ハートをドロップしやすい。" };
        enemyStr[Enemy_Pontan] =    { "Pontan\n.The strongest enemy.",                                  "ポンタン\n最強キャラ。" };

        mI->setLabelStr(language::getInstance()->getString(enemyStr[_step]));
        return true;
    }
    return false;
}