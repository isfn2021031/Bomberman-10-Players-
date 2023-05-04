#include "DxLib.h"
#include "../h/menuCursorMgr_inGame.h"
#include "../h/soundMgr.h"
#include "../h/language.h"
#include "../h/playerCounter.h"

menuCursorMgr_inGame::menuCursorMgr_inGame(int max, input_key* kIptr, bool movLast) :menuCursorMgr(max, movLast)
{
	if (kIptr != nullptr)
	{
		dynamic_kI = false;
		if (kI != nullptr)
		{
			delete kI;
			kI = nullptr;
		}
		kI = kIptr;
	}
	restInput = true;
	cursorStr = language::getInstance()->getStringVec(
		{ {"RESUME GAME","�Q�[�����Â���"}, {"START THE GAME OVER","���Ȃ���"}, {"BACK TO TITLE SCREEN","�Q�[������߂�"} });
	labelStr = language::getInstance()->getString({ "PAUSE","�|�[�Y" });
}
// 2022/09/14 ���N���X�|�C���^�ɔh���N���X�� new �����Ƃ��̃f�X�g���N�^�Ăяo���ɂ���
// https://kaworu.jpn.org/cpp/%E7%B6%99%E6%89%BF%E3%81%A8%E4%BB%AE%E6%83%B3%E3%83%87%E3%82%B9%E3%83%88%E3%83%A9%E3%82%AF%E3%82%BF#.E3.82.B5.E3.83.96.E3.82.AF.E3.83.A9.E3.82.B9.E3.81.AE.E3.83.87.E3.82.B9.E3.83.88.E3.83.A9.E3.82.AF.E3.82.BF.E3.81.8C.E5.91.BC.E3.81.B3.E5.87.BA.E3.81.95.E3.82.8C.E3.81.AA.E3.81.84.E4.BE.8B
// �悭�킩��Ȃ�������A�Ƃ肠�����f�X�g���N�^��virtual�t���Ƃ��΂����B
menuCursorMgr_inGame::~menuCursorMgr_inGame()
{
}

int menuCursorMgr_inGame::Update()
{
	if (kI != nullptr)
	{
		if (dynamic_kI)
		{
			kI->Update();
		}
		int rv_base = menuCursorMgr::output();

		if (!restInput)
		{
			if (pause)
			{
				if (rv_base > 0)
				{
					// �I�����uRESUME GAME�v
					if (cursor == 0)
					{
						pause = false;
						restInput = true;
						return -1;
					}
					// �I�����uBACK TO RULES SCREEN�v�uSTART THE GAME OVER�v
					return cursor;
				}
				// B�{�^���́uRESUME GAME�v�Ɠ�������
				if (rv_base < 0)
				{
					pause = false;
					restInput = true;
					return -1;
				}
			}
		}
		if (kI->getButtonEdge(actButton_P))
		{
			pause = !pause;

			if (pause)
			{
				soundMgr::GetInstance().Play(SE_Pause);
			}
			else
			{
				restInput = true;
				return -1;
			}
		}
	}
	return 0;
}
void menuCursorMgr_inGame::Draw(int x, int y)
{
	if (pause)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 96);
		DrawBox(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GetColor(0, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); 

		menuCursorMgr::Draw(x, y);
		//playerCounter::GetInstance().Draw();
	}
}

void menuCursorMgr_inGame::setRestInput(bool f)
{
	menuCursorMgr::setRestInput(f);

	if (f)
	{
		pause = false;
	}
}