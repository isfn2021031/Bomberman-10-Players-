#include "DxLib.h"
#include "../h/Scene_StageMaker.h"
#include "../h/soundMgr.h"
#include "../h/language.h"
#include "../h/myfunc.h"

Scene_StageMaker::Scene_StageMaker(ISceneChanger* changer) : BaseScene(changer)
{
    kIptr = new input_key(DX_INPUT_KEY_PAD1);
    mk_board = new board_make;
    mIptr = new menuCursorMgr_inGame(6, kIptr);
    mIptr->setCursorStr(language::getInstance()->getStringVec({
        {"Resume",              "つづける"},
        {"Change Start Point",  "開始位置変更"},
        {"Load",                "ロード"},
        {"Change The Default",  "デフォルトに戻す"},
        {"Save & Exit",         "セーブしてやめる"},
        {"Exit",                "やめる"}
    }));
}
Scene_StageMaker::~Scene_StageMaker()
{
    delete kIptr;
    delete mk_board;
    delete mIptr;
}

// 更新
void Scene_StageMaker::Update()
{
    BaseScene::Update();

    if (kIptr != nullptr)
    {
        kIptr->Update();
    }
    if (mIptr != nullptr)
    {
        if (!mIptr->getPause())
        {
            mk_board->Update();
        }
        int mI_rv = mIptr->Update();

        // ポーズ画面中の処理
        if (mIptr->getPause())
        {
            mIptr->setRestInput(false);
            mk_board->setSave(true);

            switch (mI_rv)
            {
            case 1:
                startPattern = (++startPattern) % StartPattern_MAX;
                mk_board->setStartPattern(startPattern);
                return;

            case 2:
                board_withGimmick::setEditFileName(getLoadFileName({ "stg file\0*.stg;\0", "Load Stage" }));
                mk_board->LoadBoardToFile();
                return;

            case 3:
                board_withGimmick::setEditFileName(FPATH_DEF_STG);
                mk_board->LoadBoardToFile();
                return;

            case 4:
                // シーンを切り替えるだけでmk_boardのデストラクタが呼ばれる。
                mSceneChanger->ChangeScene(eScene_Menu);
                return;

            case 5:
                // シーンを切り替えるだけでmk_boardのデストラクタが呼ばれる。
                mSceneChanger->ChangeScene(eScene_Menu);
                mk_board->setSave(false);
                return;

            default:
                break;
            }
        }
    }
}

// 描画
void Scene_StageMaker::Draw()
{
    mk_board->Draw();

    if (mIptr != nullptr)
    {
        if (mIptr->getCursor() == 1)
        {
            elementMgr::DrawPlayersPreview(startPattern);
        }
        mIptr->Draw(WINDOW_WIDTH * 0.4, WINDOW_HEIGHT * 0.3);
    }
}