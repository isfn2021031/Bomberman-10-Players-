#include "DxLib.h"
#include "../h/dialogue_withMenu.h"
#include "../h/soundMgr.h"
#include "../h/language.h"
#include "../h/menuCursorMgr_X.h"

dialogue_withMenu::dialogue_withMenu(string str) :dialogue(str)
{
}
dialogue_withMenu::~dialogue_withMenu()
{
	if (mI_ptr != nullptr)
	{
		delete mI_ptr;
		mI_ptr = nullptr;
	}
}

int dialogue_withMenu::Update()
{
	if (finishAnimationCnt <= 0)
	{
		if (mI_ptr != nullptr)
		{
			int retval = mI_ptr->Update();
			cur = (retval > 0) ? mI_ptr->getCursor() : retval;

			if (retval != 0)
			{
				delete mI_ptr;
				mI_ptr = nullptr;
				finishAnimationCnt = 1;
			}
		}
		else
		{
			if (txAnime_ptr != nullptr)
			{
				if (txAnime_ptr->Update())
				{
					mI_ptr = new menuCursorMgr_X(2);
					mI_ptr->setCursorStr(language::getInstance()->getStringVec(
						{
							{"YES",	"‚Í‚¢"},
							{"NO",	"‚¢‚¢‚¦"},
						}
					));
				}
			}
		}
	}
	else
	{
		if (finishAnimationCnt > finishAnimationMAX)
		{
			switch (cur)
			{
			case 0:				return 1;
			case -1:case 1:		return -1;
			default:			break;
			}
		}
		finishAnimationCnt++;
	}
	return 0;
}
void dialogue_withMenu::Draw()
{
	dialogue::Draw();

	if (mI_ptr != nullptr)
	{
		int cx = WINDOW_WIDTH * 0.375;
		int cy = WINDOW_HEIGHT / 2;

		mI_ptr->Draw(cx, cy);
	}
}