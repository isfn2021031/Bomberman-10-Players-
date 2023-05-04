#include"Dxlib.h"
#include "../h/debug.h"
#include "../h/playerCounter.h"
#include "../h/bombMgr.h"

DebugMgr::DebugMgr()
{
}
DebugMgr::~DebugMgr()
{

}

void DebugMgr::Update()
{
	keyInput();
}
void DebugMgr::Draw()
{
	if (mode != Debug_None)
	{
		DrawFormatString(WINDOW_WIDTH * 0.25, 0, GetColor(0, 0, 0), "debugMode: %d-P%d\n%.1f FPS", mode, targetID + 1, FPS);
	}
	for (int i = 0; i < dispStr.size(); i++)
	{
		int x1 = 10;
		int y1 = 10 + i * 20;
		int w = 8 * dispStr[i].size();

		DrawBox(x1, y1, x1 + w, y1 + 20, GetColor(255, 255, 255), TRUE);
		DrawFormatString(x1, y1, GetColor(0, 0, 0), dispStr[i].c_str());
	}
	dispStr.clear();
}

void DebugMgr::keyInput()
{
	// キーが押されているとデバッグモードになる。
	int Key_shiftR = CheckHitKey(KEY_INPUT_RSHIFT);

	if (Key_shiftR > OldKey_shiftR)
	{
		mode = (++mode) % Debug_MAX;
	}
	OldKey_shiftR = Key_shiftR;

	int Key_shiftL = CheckHitKey(KEY_INPUT_LSHIFT);

	if (Key_shiftL > OldKey_shiftL)
	{
		mode--;
		if (mode < 0)
		{
			mode = Debug_MAX - 1;
		}
	}
	OldKey_shiftL = Key_shiftL;

	int Key_ctrlR = CheckHitKey(KEY_INPUT_RCONTROL);

	if (Key_ctrlR > OldKey_ctrlR)
	{
		targetID = (++targetID) % PLAYER_MAX;
	}
	OldKey_ctrlR = Key_ctrlR;

	int Key_ctrlL = CheckHitKey(KEY_INPUT_LCONTROL);

	if (Key_ctrlL > OldKey_ctrlL)
	{
		targetID--;
		if (targetID < 0)
		{
			targetID = PLAYER_MAX - 1;
		}
	}
	OldKey_ctrlL = Key_ctrlL;
}