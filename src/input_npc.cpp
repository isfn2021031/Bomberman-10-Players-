#include "DxLib.h"
#include <stdlib.h>
#include <time.h>
#include "../h/myfunc.h"
#include "../h/input_npc.h"
#include "../h/player.h"
#include "../h/board_main.h"
#include "../h/bombMgr.h"
#include "../h/itemMgr.h"
#include "../h/dangerZoneForecast.h"
#include "../h/movableSearcher.h"

input_npc::input_npc(int code, int id) :input_base(code, id)
{
	playerID = id;
	rates.update = (rand() % 8) + 8; 
	rates.update_avoidance = (rand() % 10) + 1;
	reverseTurn = bool(rand() % 2);

	shuffleArray(PriorityDirections, Direction_MAX);

	// 向きの初期化
	for (int dir = 0; dir < Direction_MAX; dir++)
	{
		int pd = PriorityDirections[dir];
		if (centerIdx_ptr != nullptr)
		{
			vector2D<int> newIdx = *centerIdx_ptr + getDirectionSigns(pd);
			board_main* pBoard = board_main::getInstance();

			// 移動しようとしている場所が地面なら
			if (pBoard->checkGroundFromIndex(newIdx))
			{
				direction = pd;
				break;
			}
		}
	}
	randDatalist.clear();

	srand(time(NULL) + id);

	int sz = 1024 + 64 * (rand() % 1024);

	for (int i = 0; i < sz; i++)
	{
		randDatalist.push_back(rand() + id + 1);
	}
	superEscape = (getRandData() % 2);
}

void input_npc::Update()
{
	if (centerIdx_ptr != nullptr)
	{
		frontIdx = { centerIdx_ptr->x + getDirectionSigns(direction).x,  centerIdx_ptr->y + getDirectionSigns(direction).y };

		createCells();
		createConnectedCnt(*centerIdx_ptr);
		dangerCnt = 0;

		if (dangerZoneForecast::GetInstance().getDangerZone(*centerIdx_ptr))
		{
			dangerCnt = dangerZoneForecast::GetInstance().getRiskCnt(*centerIdx_ptr);
		}
		for (int d = 0; d < Direction_MAX; d++)
		{
			dangerFollowCnt[d] = 0;
			vector2D<int> newIdx = *centerIdx_ptr + getDirectionSigns(d);

			if (dangerZoneForecast::GetInstance().getDangerZone(newIdx))
			{
				dangerFollowCnt[d] = dangerZoneForecast::GetInstance().getRiskCnt(newIdx);
			}
		}
		itemParam = createItemParam(*centerIdx_ptr);

		if (dangerCnt > rates.danger_basisPoint)
		{
			old_dangerCnt = FC_BOMB_BLAST * rates.escape;
		}
		if (old_dangerCnt > 0)
		{
			old_dangerCnt--;
		}
		if (createFlags().escape)
		{
			if (FrameCounter % rates.update_avoidance == 0)
			{
				direction = searchSafeDirection(*centerIdx_ptr);
			}
			buttons.rocket = (dangerCnt && connectCntMax <= 0);
		}
		else
		{
			if (FrameCounter % rates.update * 2 == 0)
			{
				direction = buttons.bomb ? Direction_None : createDirection(direction);
			}
			else if (direction != Direction_None)
			{
				turn = false;
				int nd = rand() % Direction_MAX;
				// 移動しようとしている場所が危険地帯なら
				if (connectedCnt[direction] < 1 && connectedCnt[nd])
				{
					turn = true;
					direction = nd;
				}
			}
		}
	}
	FrameCounter++;

	buttons.bomb = false;
	buttons.remote = false;
	buttons.punch = false;
	
	npcFlags flags = createFlags();

	if (!flags.escape)
	{
		board_main* pBoard = board_main::getInstance();

		if (pBoard != nullptr)
		{
			cellParam frontCell = pBoard->getCell(frontIdx);
			
			// 向いてる方向にソフトブロックがあれば確定でボムセット
			if (frontCell.type == CellId_SoftBlock)
			{
				setButton_bomb(true);
				return;
			}
		}
	}
	if (FrameCounter % rates.update == 0)
	{
		if (flags.bombs && itemParam.mostCnt <= 0)
		{
			if (setButton_bomb())
			{
				return;
			}
		}		
		if (flags.remote)
		{
			if (calcProbability(rates.remote))
			{
				buttons.remote = true;
				return;
			}
		}
		if (dangerCnt > rates.danger_basisPoint)
		{
			if (setButton_punch())
			{
				return;
			}
		}
	}
}
void input_npc::Draw()
{
	for (int d = 0; d < Direction_MAX; d++)
	{
		if (centerIdx_ptr != nullptr)
		{
			vector2D<int> disp = *centerIdx_ptr;
			disp.x += getDirectionSigns(d).x;
			disp.y += getDirectionSigns(d).y;
			disp.x *= CELL_SIZE;
			disp.y *= CELL_SIZE;
			disp.x += BASIS_PX;
			disp.y += BASIS_PY;
			DrawFormatString(disp.x, disp.y, GetColor(200, 20, 0), " 「%d」", connectedCnt[d]);
		}
	}
}
void input_npc::DrawStatus()
{
	int interval = 20;
	int dx = WINDOW_WIDTH * 0.15;
	int dy = WINDOW_HEIGHT * 0.92;

	DrawFormatString(dx, dy, GetColor(0, 0, 0),
		"--- %s ---\n,"
		"update={ %d, %d }, eyes: %d, bombs: %.1f, punch: %.1f, remote: %.1f, turn: %.1f, escape: %.1f",
		patternStr.c_str(), rates.update, rates.update_avoidance, rates.eyes, rates.bomb, rates.punch, rates.remote, rates.turn, rates.escape);
}

void input_npc::createCells()
{
	board_main* pBoard = board_main::getInstance();
	
	if (pBoard != nullptr)
	{
		pBoard->getCells(cells_copy);
	}

	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			// ボムが置かれている場所も壁とする。
			if (bombMgr::getExistenceOnBoard({ x,y }))
			{
				cells_copy[y][x] = CellId_HardBlock;
			}
		}
	}
}
void input_npc::createConnectedCnt(vector2D<int> _idx)
{
	board_main* pBoard = board_main::getInstance();

	if (pBoard != nullptr)
	{
		int chkCell = board_main::getInstance()->getCell(_idx).type;
		int tmp[BOARD_HEIGHT][BOARD_WIDTH] = { 0 };
		bool noCnt[BOARD_HEIGHT][BOARD_WIDTH] = { false };

		for (int y = 0; y < BOARD_HEIGHT; y++)
		{
			for (int x = 0; x < BOARD_WIDTH; x++)
			{
				tmp[y][x] = cells_copy[y][x];
				noCnt[y][x] = false;

				if (dangerZoneForecast::GetInstance().getDangerZone({ x,y }))
				{
					noCnt[y][x] = true;
				}
			}
		}
		movableSearcher ms_c(cells_copy, noCnt);
		ms_c.setCheckCellId(chkCell);
		connectCntCenter = ms_c.getConnectedCnt(_idx);

		// 今いる場所はカウントしない
		tmp[_idx.y][_idx.x] = CellId_HardBlock;

		connectCntMax = 0;
		connectCntMin = Direction_MAX;

		for (int d = 0; d < Direction_MAX; d++)
		{
			vector2D<int> newIdx = _idx;
			newIdx.x += getDirectionSigns(d).x;
			newIdx.y += getDirectionSigns(d).y;

			movableSearcher ms(tmp, noCnt);
			ms.setCheckCellId(chkCell);
			int n = ms.getConnectedCnt(newIdx);
			connectedCnt[d] = n;

			if (n > 0)
			{
				if (n < connectCntMin)
				{
					connectCntMin = n;
				}
				if (n > connectCntMax)
				{
					connectCntMax = n;
				}
			}
		}
	}
}

bool input_npc::getButton(int n)
{
	switch (n)
	{
	case actButton_A:		return buttons.bomb;
	case actButton_B:		return buttons.punch;
	case actButton_L:		return buttons.rocket;
	case actButton_R:		return buttons.remote;
	default:				break;
	}
	return false;
}
bool input_npc::getButtonEdge(int n)
{
	switch (n)
	{
	case actButton_R:		return buttons.remote;
	case actButton_A:		return buttons.bomb;
	case actButton_B:		return buttons.punch;
	default:	break;
	}
	return false;
}
bool input_npc::setButton_bomb(bool f)
{
	for (int d = 0; d < Direction_MAX; d++)
	{
		if (dangerFollowCnt[d] > rates.danger_basisPoint * 2)
		{
			return false;
		}
	}
	double rate_real = f ? 1.0 : rates.bomb;

	if (calcProbability(rate_real))
	{
		if (connectCntMin > 1)
		{
			buttons.bomb = true;
			return true;
		}
	}
	return false;
}
bool input_npc::setButton_punch()
{
	if (calcProbability(rates.punch))
	{
		if (direction != Direction_None)
		{
			if (bombMgr::getExistenceOnBoard(frontIdx))
			{
				buttons.punch = true;
				return true;
			}
		}
	}
	return false;
}

int input_npc::checkSafeDirection(vector2D<int> _idx, int oldDirection, int mostSafeCnt, int baseCost)
{
	board_main* pBoard = board_main::getInstance();

	if (pBoard != nullptr)
	{
		// 進もうとしている場所が壁なら移動できない
		if (pBoard->checkGroundFromIndex(_idx))
		{
			int oldSafeCnt = mostSafeCnt;
			int riskcnt = dangerZoneForecast::GetInstance().getRiskCnt(_idx, baseCost);
			if (riskcnt < mostSafeCnt)
			{
				mostSafeCnt = riskcnt;
			}

			for (int d = 0; d < Direction_MAX; d++)
			{
				int realD = PriorityDirections[d];

				if (realD != getReverseDirection(oldDirection))
				{
					vector2D<int> newIdx = _idx;

					newIdx.x += getDirectionSigns(realD).x;
					newIdx.y += getDirectionSigns(realD).y;

					// 遠くに移動するほどリスクが高いとする。
					riskcnt = getRiskCnt(newIdx, baseCost + 1);

					// 進もうとしている場所が壁なら移動できない
					if (pBoard->checkGroundFromIndex(newIdx))
					{
						if (riskcnt < mostSafeCnt)
						{
							mostSafeCnt = riskcnt;
						}
					}
				}
			}
			if (mostSafeCnt < oldSafeCnt)
			{
				return mostSafeCnt;
			}
		}
	}
	return -1;
}
int input_npc::createDirection(int realD)
{
	int rv = realD;
	int mostCnt = 0;

	if (centerIdx_ptr != nullptr)
	{
		for (int dir = 0; dir < Direction_MAX; dir++)
		{
			int pd = PriorityDirections[dir];
			if (!reverseTurn && pd == getReverseDirection(realD))
			{
				rv = realD;
			}
			else if (calcProbability(rates.turn) || turn)
			{
				vector2D<int> newIdx = *centerIdx_ptr + getDirectionSigns(pd);
				board_main* pBoard = board_main::getInstance();

				// 移動しようとしている場所が安全な地面なら
				if (pBoard->checkGroundFromIndex(newIdx))
				{
					if (!dangerZoneForecast::GetInstance().getDangerZone(newIdx) && connectedCnt[pd] >= 1)
					{
						int nowCnt = itemPanel::getStrength(itemMgr::GetExistenceLightweight(newIdx));
						if (mostCnt <= nowCnt)
						{
							mostCnt = nowCnt;
							rv = pd;
						}
					}
				}
			}
		}
	}
	if (rv != realD)
	{
		if (itemParam.direction != Direction_None)
		{
			return itemParam.direction;
		}
	}
	return rv;
}

int input_npc::searchSafeDirection(vector2D<int> _idx)
{
	int rv = Direction_None;
	int mostSafeCnt = getRiskCnt(_idx);

	for (int d = 0; d < Direction_MAX; d++)
	{
		int realD = PriorityDirections[d];
		vector2D<int> newIdx = _idx;

		for (int i = 0; i < rates.eyes; i++)
		{
			newIdx.x += getDirectionSigns(realD).x;
			newIdx.y += getDirectionSigns(realD).y;

			int tmp = checkSafeDirection(newIdx, realD, mostSafeCnt, i);

			if (tmp == -1)
			{
				break;
			}
			if (mostSafeCnt > tmp)
			{
				mostSafeCnt = tmp;
				rv = realD;
			}
		}
	}
	if (superEscape)
	{
		if (dangerCnt > rates.danger_basisPoint && connectCntMax <= 0 && rv == Direction_None)
		{
			rv = getRandData() % Direction_MAX;
		}
	}
	return rv;
}
int input_npc::getRiskCnt(vector2D<int> _idx, int cost)
{
	int cnt1 = dangerZoneForecast::GetInstance().getRiskCnt(_idx, cost);
	int cnt2 = itemPanel::getStrength(itemMgr::GetExistenceLightweight(_idx));

	if ((cnt1 - cnt2) > 0)
	{
		return	cnt1 - cnt2;
	}
	return cnt1;
}
int input_npc::getRandData()
{
	randIdx++;

	if (randIdx >= randDatalist.size())
	{
		randIdx = 0;
	}
	return randDatalist[randIdx];
}
int input_npc::getDirection()
{
	if (!buttons.bomb)
	{
		return input_base::getDirection();
	}
	return Direction_None;
}

npcFlags input_npc::createFlags()
{
	npcFlags rv;
	rv.bombs = true;
	rv.remote = true;
	rv.escape = false;

	if (dangerCnt > rates.danger_basisPoint || old_dangerCnt)
	{
		rv.bombs = false;
		rv.remote = false;
		rv.escape = true;
	}
	return rv;
}

input_npc::item_knowlage input_npc::createItemParam(vector2D<int> _idx)
{
	item_knowlage rv;
	rv.direction = Direction_None;
	rv.mostCnt = 0;

	for (int d = 0; d < Direction_MAX; d++)
	{
		int realD = PriorityDirections[d];
		vector2D<int> newIdx = _idx;

		for (int i = 1; i < rates.eyes; i++)
		{
			newIdx.x = newIdx.x + getDirectionSigns(realD).x * i;
			newIdx.y = newIdx.y + getDirectionSigns(realD).y * i;

			int nowCnt = itemPanel::getStrength(itemMgr::GetExistenceLightweight(newIdx));

			if (!board_main::getInstance()->checkGroundFromIndex(newIdx) ||
				nowCnt < 0 || dangerZoneForecast::GetInstance().getDangerZone(newIdx))
			{
				break;
			}
			if (nowCnt > rv.mostCnt)
			{
				rv.mostCnt = nowCnt;
				rv.direction = realD;
			}
		}
	}
	return rv;
}
