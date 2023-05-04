#include "DxLib.h"
#include "../h/buff_countdown.h"
#include "../h/player.h"
#include "../h/soundMgr.h"

int buff_countdown::GH_counter = -1;

buff_countdown::buff_countdown(int fcMAX) :buff_base(fcMAX)
{
	if (GH_counter == -1)
	{
		GH_counter = LoadGraph("textures/countdown.png");
	}
}
buff_countdown::~buff_countdown()
{

}

void buff_countdown::bf_main()
{
	if (second_left != second_old)
	{
		if (0 < second_left && second_left < 10)
		{
			soundMgr::GetInstance().Play(SE_CountDown);
		}
		else if (second_left <= 0)
		{
			soundMgr::GetInstance().Play(SE_Alarm);
		}
	}
}
void buff_countdown::Draw(int dx, int dy)
{
	int drawX = BASIS_PX + dx;
	int drawY = dy - CELL_SIZE * 1.75;
	DrawGraph(drawX, drawY, GH_counter, TRUE);
	DrawFormatString(drawX + CELL_SIZE * 0.3, drawY + CELL_SIZE * 0.45, GetColor(0, 0, 0), "%2d", second_left);
}