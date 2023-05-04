#include "../h/bombBlastMgr.h"
#include "../h/dangerZoneForecast.h"

bombBlastMgr::bombBlastMgr()
{
}
bombBlastMgr::~bombBlastMgr()
{
}

void bombBlastMgr::Update()
{
	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			// ”š•—‚ÌXVˆ—
			explosion[y][x].setHitBoxIndex({ x, y });
			explosion[y][x].Update();
		}
	}
}
void bombBlastMgr::Draw()
{
	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			explosion[y][x].Draw();
		}
	}
	dangerZoneForecast::GetInstance().Draw();
}
bombBlast* bombBlastMgr::getBlast(vector2D<int> _idx)
{
	if (element::checkOffScreenIdx(_idx))
	{
		return nullptr;
	}
	if (!explosion[_idx.y][_idx.x].getExistence())
	{
		return nullptr;
	}
	return &explosion[_idx.y][_idx.x];
}
/*int bombBlastMgr::getBlastPlayerID(vector2D<int> _idx)
{
	if (element::checkOffScreenIdx(_idx))
	{
		return -1;
	}
	if (!explosion[_idx.y][_idx.x].getExistence())
	{
		return -1;
	}
	return explosion[_idx.y][_idx.x].getPlayerID();
}*/
