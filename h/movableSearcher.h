#pragma once
#include "../h/global.h"

class movableSearcher
{
public:
	movableSearcher				(int _cells[BOARD_HEIGHT][BOARD_WIDTH], bool noCnt[BOARD_HEIGHT][BOARD_WIDTH]);
	void	createCost			(vector2D<int> idx, vector2D<int> goalIdx);
	void	setCheckCellId		(int cellId) { checkCellId = cellId; }
	int		getConnectedCnt		(vector2D<int> _idx, int _cnt = 0);
	bool	getPassed			(vector2D<int> _idx) { return passed[_idx.y][_idx.x]; }
	
private:
	int		checkCellId = 0;
	int		cells_copy[BOARD_HEIGHT][BOARD_WIDTH] = { 0 };
	int		cost[BOARD_HEIGHT][BOARD_WIDTH] = { 0 };
	// �ǔ��肪�����ăJ�E���g�����Ȃ��}�X
	bool	passed[BOARD_HEIGHT][BOARD_WIDTH] = { false };
	// �ǔ���͂Ȃ����ǃJ�E���g���Ȃ��}�X
	bool	noCnt[BOARD_HEIGHT][BOARD_WIDTH] = { false };
};