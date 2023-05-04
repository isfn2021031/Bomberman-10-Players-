#include <stdlib.h>
#include <string>
#include "DxLib.h"
#include "../h/player_base.h"
#include "../h/bombMgr.h"
#include "../h/darkness.h"
#include "../h/debug.h"
#include "../h/soundMgr.h"
#include "../h/graphicMgr_common.h"
#include "../h/RGB.h"

using namespace std;

player_base::player_base(int _playerID) :element_canMove(true)
{
	playerID = _playerID;
	teamID = playerCounter::GetInstance().getTeamID(playerID);
	nameStr = playerCounter::GetInstance().getNameString(playerID);
	setExistence(true);
	faceUp = true;
	graphIdx = rand() % 2;
	graphMgr.setPlayerID(playerID);
	graphMgr.setTeamID(teamID);
	graphMgr.setCostumeID(playerCounter::GetInstance().getProfile(playerID).costumeIDs);
}
player_base::~player_base()
{
}

void player_base::Update()
{
	element_canMove::Update();

	nowSet = false;
	movement = false;
	movement_real = false;

	if (getExistence())
	{
		// キー入力取得
		kI->Update();
		newDirection = kI->getDirection();

		vector2D<pixelPosi> newPosi = getMovePixel(getHitBoxPixel(), newDirection);

		// XかYの片方のみ移動する場合
		if ((newPosi.x - getHitBoxPixel().x) == 0 || (newPosi.y - getHitBoxPixel().y) == 0)
		{
			movement = move(newPosi);
			if (newDirection != Direction_None)
			{
				direction = newDirection;
			}
		}
		// ボムセット
		if (kI->getButtonEdge(actButton_A))
		{
			nowSet = true;
		}
	}
	if ((FrameCnt + playerID) % 16 == 0)
	{
		graphIdx = (++graphIdx) % 2;
	}
}
void player_base::Draw()
{
	int dx = BASIS_PX + getHitBoxPixel().x + (CELL_SIZE / 2);
	int dy = BASIS_PY + getHitBoxPixel().y - 14 + (CELL_SIZE / 2);

	// プレイヤーを描画する
	graphMgr.setState(pose_walk);

	int realIdx = (movement && graphIdx + 1 < plGraph_MAX) ? graphIdx + 1 : plGraph_Idle;
	graphMgr.Draw(dx, dy, exRate, direction, realIdx, graphReverse);
}
void player_base::DrawPlayerName()
{
	if (!playerCounter::GetInstance().getNpc(playerID) || DebugMgr::GetInstance().getMode() != Debug_None)
	{
		int dx = BASIS_PX + getHitBoxPixel().x + (CELL_SIZE / 2);
		int dy = BASIS_PY + getHitBoxPixel().y - CELL_SIZE * 1.25;

		if (nameStr.empty())
		{
			graphicMgr_common::GetInstance().DrawPlayerName(playerID, dx, dy, playerCounter::GetInstance().getNpc(playerID));
		}
		else
		{
			DrawBox(dx - 32, dy + 16, dx + 48, dy + 32, GetColor(0, 0, 0), TRUE);
			DrawFormatString(dx - 24, dy + 16, GetColor(255, 255, 255), nameStr.c_str());
		}
	}
}

void player_base::setExistence(bool flag)
{
	element_canMove::setExistence(flag);
	darkness::getInstance()->setExistence(playerID, flag);
}
void player_base::setHitBoxPixel(vector2D<pixelPosi> p)
{
	element_canMove::setHitBoxPixel(p);
	darkness::getInstance()->setPlayerHole(playerID, getHitBoxPixel());
}

// *&について https://teratail.com/questions/212482
bool player_base::pushBomb(element_canThrow*& elemptr, elemPushInfo ps)
{
	if (elemptr != nullptr)
	{
		elemptr->setHold(false);
		elemptr->setDirection(ps.direction);
		elemptr->setPushState(ps);
		elemptr = nullptr;
		return true;
	}
	return false;
}