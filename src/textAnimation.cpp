#include "DxLib.h"
#include "../h/textAnimation.h"

textAnimation::textAnimation(string str)
{
	String = str;
	String_disp.resize(str.size());

	kI_ptr = new input_key(DX_INPUT_KEY_PAD1);
}
textAnimation::~textAnimation()
{
	if (kI_ptr != nullptr)
	{
		delete kI_ptr;
		kI_ptr = nullptr;
	}
}

bool textAnimation::Update()
{
	if (kI_ptr != nullptr)
	{
		kI_ptr->Update();

		if (idx_str >= String.size() && kI_ptr->getButtonEdgeAll())
		{
			return true;
			//finishAnimationCnt = 1;
		}
		frameCnt++;
	}
	if (frameCnt % rate_update == 0)
	{
		if (idx_str < String.size())
		{
			idx_str++;
		}
		for (int i = 0; i < idx_str; i++)
		{
			if (i < String.size())
			{
				String_disp[i] = String[i];
			}
		}
	}
	return false;
}
void textAnimation::Draw(int dx, int dy)
{
	if (!String_disp.empty())
	{
		DrawFormatString(dx, dy, GetColor(255, 255, 255), String_disp.c_str());
	}
}