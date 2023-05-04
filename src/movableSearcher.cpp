#include "../h/movableSearcher.h"
#include "../h/element.h"

movableSearcher::movableSearcher(int _cells[BOARD_HEIGHT][BOARD_WIDTH], bool _noCnt[BOARD_HEIGHT][BOARD_WIDTH])
{
	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			cells_copy[y][x] = _cells[y][x];
			passed[y][x] = false;
			noCnt[y][x] = _noCnt[y][x];
			cost[y][x] = 0;
		}
	}
}
void movableSearcher::createCost(vector2D<int> idx, vector2D<int> goalIdx)
{
	//このマスがゴールなら終了
	if (idx == goalIdx || element::checkOffScreenIdx(idx) || element::checkOffScreenIdx(goalIdx))
	{
		return;
	}
    /* このマスを通過したことを覚えておく */
    passed[idx.y][idx.x] = true;

	for (int d = 0; d < Direction_MAX; d++)
	{
		vector2D<int> newIdx = idx + getDirectionSigns(d);

		if (!element::checkOffScreenIdx(newIdx))
		{
			if (newIdx.x != idx.x || newIdx.y != idx.y)
			{
				if (cells_copy[newIdx.y][newIdx.x] == checkCellId)
				{
					if (!passed[newIdx.y][newIdx.x])
					{
						/* 次のマスからゴールまで移動させる */
						createCost(newIdx, goalIdx);
					}
				}
			}
		}
	}
    /* このマスを通過したことを忘れる */
    passed[idx.y][idx.x] = false;
}

int movableSearcher::getConnectedCnt(vector2D<int> _idx, int _cnt)
{
	// チェック済ならば加算せずに返す。
	if (passed[_idx.y][_idx.x] || cells_copy[_idx.y][_idx.x] != checkCellId)
	{
		return _cnt;
	}
	if (!noCnt[_idx.y][_idx.x])
	{ 
		// 加算
		_cnt++;
	}
	// 加算した場所をチェック済みにする
	passed[_idx.y][_idx.x] = true;

	// 各方向チェックする。
	for (int d = 0; d < Direction_MAX; d++)
	{
		vector2D<int> newIdx = { _idx.x + getDirectionSigns(d).x, _idx.y + getDirectionSigns(d).y };

		if (!element::checkOffScreenIdx(newIdx))
		{
			// 再帰（加算される場合があるので、戻り値 _cntを更新しておく。）
			_cnt = getConnectedCnt(newIdx, _cnt);
		}
	}
	return _cnt;
}
