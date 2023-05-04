#include "../h/suddenDeath_rota.h"

suddenDeath_rota::suddenDeath_rota(int pattern, bool se) :suddenDeath(pattern, se)
{
	rate_update = 6;
}
void suddenDeath_rota::Update()
{
	rate_update = (blockCnt > ((BOARD_HD * BOARD_WD))) ? 6 : 5;
	
	suddenDeath::Update();
}
void suddenDeath_rota::createSetIndex()
{
	vector2D<int> oldIndex = setIndex;

	setIndex.x += getDirectionSigns(direction).x;
	setIndex.y += getDirectionSigns(direction).y;

	// 衝突したら終了命令
	if (getFlag(setIndex))
	{
		// ターンできなかったら終了
		if (!turn(oldIndex))
		{
			active = false;
		}
	}
}