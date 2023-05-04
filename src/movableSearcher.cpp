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
	//���̃}�X���S�[���Ȃ�I��
	if (idx == goalIdx || element::checkOffScreenIdx(idx) || element::checkOffScreenIdx(goalIdx))
	{
		return;
	}
    /* ���̃}�X��ʉ߂������Ƃ��o���Ă��� */
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
						/* ���̃}�X����S�[���܂ňړ������� */
						createCost(newIdx, goalIdx);
					}
				}
			}
		}
	}
    /* ���̃}�X��ʉ߂������Ƃ�Y��� */
    passed[idx.y][idx.x] = false;
}

int movableSearcher::getConnectedCnt(vector2D<int> _idx, int _cnt)
{
	// �`�F�b�N�ςȂ�Ή��Z�����ɕԂ��B
	if (passed[_idx.y][_idx.x] || cells_copy[_idx.y][_idx.x] != checkCellId)
	{
		return _cnt;
	}
	if (!noCnt[_idx.y][_idx.x])
	{ 
		// ���Z
		_cnt++;
	}
	// ���Z�����ꏊ���`�F�b�N�ς݂ɂ���
	passed[_idx.y][_idx.x] = true;

	// �e�����`�F�b�N����B
	for (int d = 0; d < Direction_MAX; d++)
	{
		vector2D<int> newIdx = { _idx.x + getDirectionSigns(d).x, _idx.y + getDirectionSigns(d).y };

		if (!element::checkOffScreenIdx(newIdx))
		{
			// �ċA�i���Z�����ꍇ������̂ŁA�߂�l _cnt���X�V���Ă����B�j
			_cnt = getConnectedCnt(newIdx, _cnt);
		}
	}
	return _cnt;
}
