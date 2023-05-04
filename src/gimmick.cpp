#include "../h/gimmick.h"
#include "../h/playerCounter.h"
#include "../h/board_main.h"

gimmick::gimmick()
{
	direction = Direction_None;
}
gimmick::~gimmick()
{
}

void gimmick::Update()
{
	element::Update();

	hitPl_old = hitPl;
	hitPl = false;

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		hitPlayers[i] = 0;
	}
	for (int i = 0; i < playerCounter::GetInstance().getTotal(); i++)
	{
		int id = playerCounter::GetInstance().getActiveID(i);

		if (checkHitPlayerCenterIndex(id))
		{
			hitPl = true;
			hitPlayers[id] = true;
		}
	}
}

void gimmick::setDirection(int _direction)
{
	element_accessableBoard::setDirection(_direction);

	board_main* pBoard = board_main::getInstance();
	if (pBoard != nullptr)
	{
		pBoard->setDirection(HitBoxCenterIndex, _direction);
	}
}