#include <set>
#include <string>
#include "DxLib.h"
#include "../h/configLoader.h"
#include "../h/configMgr_players.h"

using namespace std;

configMgr_players::configMgr_players() :configMgr(PLAYER_MAX)
{
    BasisDY = (int)(WINDOW_HEIGHT * 0.15);
    setSave(true, FILEPATH_PL);
    
    configLoader cl(saveFileName, stepMAX);

    for (int i = 0; i < stepMAX; i++)
    {
        menuData mData;
        const int max = ctr_MAX - 1;
        mData.Max = (i < GetJoypadNum()) ? ((i == 0) ? max : max - 1) : max - 2;
        mData.labelStr = { "  Player" + to_string(i + 1) + " :","  �v���C���[" + to_string(i + 1) + " :" };
        mData.cursorStr = 
        { {"Empty","�Ȃ�"}, {"COM","COM"}, {"Keyboard","�L�[�{�[�h"}, {"Mouse","�}�E�X"}, {"Gamepad","�Q�[���p�b�h"}, {"KeyPad","�L�[�{�[�h���Q�[���p�b�h"}, {"Network", "�l�b�g���[�N"}};
        mData.guideStrSingle = { "Choose player's controller and team.", "�v���C���[�̃R���g���[����ύX���܂��B"};
        mData.cursor = cl.getConfig(i);
        setData(i, mData);
    }
    mallocMI(0);
}
configMgr_players::~configMgr_players()
{
    for (int s = 0; s < stepMAX; s++)
    {
        if (mVec[s].cursor == ctr_Network)
        {
            mVec[s].cursor = ctr_Empty;
        }
    }
}

int configMgr_players::Update()
{
    int oldStep = step;
    int rv = configMgr::Update();
    playerCounter::GetInstance().setPlayerCtr(step, mI->getCursor());
    
    return rv;
}
void configMgr_players::Draw()
{
    configMgr::Draw();
}