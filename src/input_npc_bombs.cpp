#include "../h/input_npc_bombs.h"
#include "../h/dangerZoneForecast.h"

input_npc_bombs::input_npc_bombs(int code, int id) :input_npc(code, id)
{
	patternStr = "bombs";

	rates.remote = 1.0;
	rates.punch = 0.9;
	rates.turn = 0.05 * (rand() % 3 + 1);
	rates.escape = 1.15;

	rates.eyes += rand() % 10;
	rates.danger_basisPoint = rand() % 40;
}
void input_npc_bombs::Update()
{
	input_npc::Update();

	if (getButtonEdge(actButton_A))
	{
		old_flagCnt = Interval;
		return;
	}
	if (old_flagCnt > 0)
	{
		old_flagCnt--;
		return;
	}
	if (dangerCnt <= rates.danger_basisPoint && !old_dangerCnt)
	{
		if (direction_real == Direction_None || old_flagCnt)
		{
			rates.bomb = 1.0;
			return;
		}
	}
	if (connectCntMax >= 8)
	{
		rates.bomb = 1.0;
		return;
	}
	rates.bomb = 0.4;
}

bool input_npc_bombs::setButton_bomb(bool f)
{
	if (centerIdx_ptr != nullptr)
	{
		if (dangerZoneForecast::GetInstance().getSensitiveCnt(*centerIdx_ptr) > (int)(FC_BOMB_COOLTIME * 0.4))
		{
			return false;
		}
	}
	return input_npc::setButton_bomb(f);
}

npcFlags input_npc_bombs::createFlags()
{
	npcFlags rv;
	rv.bombs = true;
	rv.remote = true;
	rv.escape = false;

	if (dangerCnt > rates.danger_basisPoint || old_dangerCnt)
	{
		rv.escape = true;
	}
	if (dangerCnt > rates.danger_basisPoint)
	{
		rv.bombs = false;
		rv.remote = false;
	}
	return rv;
}