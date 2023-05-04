#include "../h/elementMgr.h"

const vector2D<int>	elementMgr::StartPositions[StartPattern_MAX][PLAYER_MAX] =
{
	{
		{1,	1},
		{BOARD_WD - 2, BOARD_HD - 2},
		{BOARD_WD - 2, 1},
		{1,BOARD_HD - 2},
		{5,3},
		{BOARD_WD - 6,BOARD_HD - 4},
		{BOARD_WD - 6,3},
		{5,BOARD_HD - 4},
		{1,(BOARD_HD / 2) - 1},
		{BOARD_WD - 2, (BOARD_HD / 2) + 1},
	},
	{
		{1,(BOARD_HD / 2) - 3},
		{BOARD_WD - 2, (BOARD_HD / 2) - 1},
		{BOARD_WD - 2, (BOARD_HD / 2) + 3},
		{1,(BOARD_HD / 2) + 1},

		{5,1},
		{BOARD_WD - 6, BOARD_HD - 2},
		{BOARD_WD - 6,1},
		{5,				BOARD_HD - 2},
		{BOARD_WD / 2,	1},
		{BOARD_WD / 2,	BOARD_HD - 2},
	},
	{
		{1,				(BOARD_HD / 2) - 2},
		{BOARD_WD - 2,	(BOARD_HD / 2) - 2},
		{BOARD_WD - 2,	(BOARD_HD / 2) + 2},
		{1,				(BOARD_HD / 2) + 2},
		{5,				1},
		{BOARD_WD - 6,	BOARD_HD - 2},
		{BOARD_WD - 6,	1},
		{5,BOARD_HD - 2},
		{BOARD_WD / 2,	1},
		{BOARD_WD / 2,	BOARD_HD - 2},
	},
	{
		{1,	1},
		{BOARD_WD - 2, BOARD_HD - 2},
		{BOARD_WD - 2, 1},
		{1,BOARD_HD - 2},
		{5,1},
		{BOARD_WD - 6, BOARD_HD - 2},
		{BOARD_WD - 6,1},
		{5,BOARD_HD - 2},
		{BOARD_WD / 2, 1},
		{BOARD_WD / 2, BOARD_HD - 2},
	},
	{
		{5,				5},
		{7,				5},
		{BOARD_WD / 2,	5},
		{BOARD_WD - 8,	5},
		{BOARD_WD - 6,	5},
		{5,				7},
		{7,				7},
		{BOARD_WD / 2,	7},
		{BOARD_WD - 8,	7},
		{BOARD_WD - 6,	7},
	},
	{
		{5,				1},
		{7,				1},
		{BOARD_WD / 2,	1},
		{BOARD_WD - 8,	1},
		{BOARD_WD - 6,	1},
		{5,				3},
		{7,				3},
		{BOARD_WD / 2,	3},
		{BOARD_WD - 8,	3},
		{BOARD_WD - 6,	3},
	},	
	{
		{5,				BOARD_HD - 4},
		{7,				BOARD_HD - 4},
		{BOARD_WD / 2,	BOARD_HD - 4},
		{BOARD_WD - 8,	BOARD_HD - 4},
		{BOARD_WD - 6,	BOARD_HD - 4},
		{5,				BOARD_HD - 2},
		{7,				BOARD_HD - 2},
		{BOARD_WD / 2,	BOARD_HD - 2},
		{BOARD_WD - 8,	BOARD_HD - 2},
		{BOARD_WD - 6,	BOARD_HD - 2},
	},
	{
		{1,				BOARD_HD - 2},
		{3,				BOARD_HD - 2},
		{5,				BOARD_HD - 2},
		{7,				BOARD_HD - 2},
		{BOARD_WD - 8,	BOARD_HD - 2},
		{BOARD_WD - 6,	BOARD_HD - 2},
		{BOARD_WD - 4,	BOARD_HD - 2},
		{BOARD_WD - 2,	BOARD_HD - 2},
		{BOARD_WD / 2,	BOARD_HD - 2},
		{BOARD_WD / 2,	BOARD_HD - 4},
	},
};
graphicMgr_bomberman* elementMgr::bomberGraph[PLAYER_MAX] = { nullptr };

elementMgr::elementMgr()
{
	if (bomberGraph[0] == nullptr)
	{
		for (int i = 0; i < PLAYER_MAX; i++)
		{
			bomberGraph[i] = new graphicMgr_bomberman;
			bomberGraph[i]->setPlayerID(i);
		}
	}
	clearExistenceBoard();
}
elementMgr::~elementMgr()
{
	for (int i = 0; i < ELEMENT_MAX; i++)
	{
		if (elements[i] != nullptr)
		{
			delete elements[i];
		}
	}
}

void elementMgr::Update()
{
	clearExistenceBoard();

	int tmpNum = 0;
	
	for (int e = 0; e < ELEMENT_MAX; e++)
	{
		if (elements[e] != nullptr)
		{
			if (elements[e]->getExistence())
			{
				tmpNum++;
				existenceBoard[elements[e]->getHitBoxCenterIndex().y][elements[e]->getHitBoxCenterIndex().x] = true;
			}
			elements[e]->Update();
		}
	}
	existenceNum = tmpNum;
}
void elementMgr::Draw()
{
	for (int b = 0; b < capacity; b++)
	{
		if (elements[b] != nullptr)
		{
			elements[b]->Draw();
		}
	}
}
void elementMgr::DrawPlayersPreview(int pattern)
{
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (bomberGraph[i] != nullptr)
		{
			vector2D<int> dispIdx = { StartPositions[pattern][i].x + BOARD_SHIFT_X, StartPositions[pattern][i].y + BOARD_SHIFT_Y };
			bomberGraph[i]->setPlayerID(i);
			bomberGraph[i]->Draw(BASIS_PX + dispIdx.x * CELL_SIZE + (CELL_SIZE / 2), BASIS_PY + dispIdx.y * CELL_SIZE + (CELL_SIZE / 3));
		}
	}
}

void elementMgr::clearExistenceBoard()
{
	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			existenceBoard[y][x] = false;
		}
	}
}

void elementMgr::setPlayerID(int id)
{
	playerID = id;

	for (int i = 0; i < ELEMENT_MAX; i++)
	{
		elements[i]->setPlayerID(playerID);
	}
}
void elementMgr::setCapacity(int n)
{
	capacity = (n > capacityMAX) ? capacityMAX : n;
}

element* elementMgr::setNewElement(vector2D<int> _idx)
{
	if (!element::checkOffScreenIdx(_idx))
	{
		// 最大数に達していないかつ置こうとしているマスに既に存在しないなら
		if ((existenceNum < capacity) && !existenceBoard[_idx.y][_idx.x])
		{
			for (int b = 0; b < capacity; b++)
			{
				if (elements[b] != nullptr)
				{
					if (!elements[b]->getExistence())
					{
						//existenceNum++;
						existenceBoard[_idx.y][_idx.x] = true;
						elements[b]->setHitBoxIndex(_idx);
						elements[b]->setPlayerID(playerID);
						return elements[b];
					}
				}
			}
		}
	}
	return nullptr;
}
element* elementMgr::getElementFromIndex(vector2D<int> idx)
{
	for (int e = 0; e < capacity; e++)
	{
		if (elements[e] != nullptr)
		{
			if (elements[e]->getExistence() && elements[e]->getHitBoxCenterIndex() == idx)
			{
				return elements[e];
			}
		}
	}
	return nullptr;
}

bool elementMgr::setExistenceBoard(vector2D<int> idx)
{
	if (!element::checkOffScreenIdx(idx))
	{
		existenceBoard[idx.y][idx.x] = true;
		return true;
	}
	return false;
}
bool elementMgr::getExistenceBoard(vector2D<int> idx)
{
	if (!element::checkOffScreenIdx(idx))
	{
		return existenceBoard[idx.y][idx.x];
	}
	return false;
}