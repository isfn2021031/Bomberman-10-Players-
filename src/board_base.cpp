#include <stdlib.h>
#include <time.h>
#include <string>
#include <fstream>

#include "DxLib.h"
#include "../h/myfunc.h"
#include "../h/board_base.h"
#include "../h/debug.h"

using namespace std;

int board_base::GH_cells[CellStyle_MAX][CellVisual_MAX] = { -1 };

board_base::board_base()
{
	if (GH_cells[0][0] == -1)
	{
		string FileName = "textures/cells.png";
		int tmp[CellStyle_MAX * CellVisual_MAX];
		LoadDivGraph(FileName.c_str(), CellStyle_MAX * CellVisual_MAX, CellVisual_MAX, CellStyle_MAX, CELL_SIZE, CELL_SIZE, tmp);

		for (int y = 0; y < CellStyle_MAX; y++)
		{
			for (int x = 0; x < CellVisual_MAX; x++)
			{
				GH_cells[y][x] = tmp[CellVisual_MAX * y + x];
			}
		}
	}
	clearCells();
}
board_base::~board_base()
{
}

void board_base::Update()
{
	if (DebugMgr::GetInstance().getMode() == Debug_Cheat)
	{
		if (CheckHitKey(KEY_INPUT_DELETE) == 1)
		{
			fireAllSoftBlocks();
		}
	}
}
void board_base::Draw()
{
	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			int drawX = BASIS_PX + x * CELL_SIZE;
			int drawY = BASIS_PY + y * CELL_SIZE;
			DrawGraph(drawX, drawY, GH_cells[cellStyle][convertCellIdToCellVisual({ x,y }, cells[y][x].type)], TRUE);
			SetDrawBright(255, 255, 255);
		}
	}
	if (DebugMgr::GetInstance().getMode() == Debug_Global)
	{
		for (int y = 0; y < BOARD_HEIGHT; y++)
		{
			for (int x = 0; x < BOARD_WIDTH; x++)
			{
				int dx = BASIS_PX + x * CELL_SIZE;
				int dy = BASIS_PY + y * CELL_SIZE;

				if (cells[y][x].dug)
				{
					DrawBox(dx, dy, dx + CELL_SIZE, dy + CELL_SIZE, GetColor(40, 90, 200), TRUE);
					DrawFormatString(dx, dy, GetColor(0, 0, 0), "{%d,%d}", x, y);
				}
			}

		}
	}
}

void board_base::setCellStyleGround(int _cellStyle)
{
	groundId = (_cellStyle == CellStyle_SlipSliding) ? CellId_IceGround : CellId_Ground;
	setGround();
}
void board_base::setCell(vector2D<int> idx, int cellId)
{
	if (cellId == -1)
	{
		cellId = groundId;
	}
	cells[idx.y][idx.x].type = cellId;
}
void board_base::setDirection(vector2D<int> idx, int d)
{
	if (!checkOffScreen(idx))
	{
		cells[idx.y][idx.x].direction = d;
	}
}

void board_base::setWall()
{
	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			// ステージ生成
			
			if (x <= BOARD_SHIFT_X || y <= BOARD_SHIFT_Y
				|| x >= (BOARD_SHIFT_X + BOARD_WD - 1) || y >= (BOARD_SHIFT_Y + BOARD_HD - 1))
			{
				cells[y][x].type = CellId_HardBlock;
			}
		}
	}
}
void board_base::setGround()
{
	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			setCell({ x,y });
		}
	}
}
void board_base::setHardBlocks(int _cell)
{
	for (int y = BOARD_SHIFT_Y + 2; y < BOARD_HEIGHT - 3; y += 2)
	{
		for (int x = BOARD_SHIFT_X + 2; x < BOARD_WIDTH - 3; x += 2)
		{
			cells[y][x].type = _cell;
		}
	}
}

void board_base::fireAllSoftBlocks()
{
	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			if (cells[y][x].type == CellId_SoftBlock)
			{
				cells[y][x].type = CellId_FiredSoftBlock;
			}
		}
	}
}
void board_base::getCells(int wCells[BOARD_HEIGHT][BOARD_WIDTH])
{
	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			wCells[y][x] = cells[y][x].type;
		}
	}
}
void board_base::clearCells()
{
	cellParam clearValue;
	clearValue.type = CellId_Ground;
	clearValue.dug = false;

	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			cells[y][x] = clearValue;
			setCell({ x,y });
		}
	}
}

bool board_base::checkGround(int _cell)
{
	switch (_cell)
	{
	case CellId_Ground:
	case CellId_IceGround:
		return true;

	default:
		break;
	}
	return false;
}
bool board_base::checkGroundFromIndex(vector2D<int>_idx, bool withBomb)
{
	if (!checkOffScreen(_idx))
	{
		return (checkGround(cells[_idx.y][_idx.x].type));
	}
	return false;
}
bool board_base::checkOffScreen(vector2D<int> _idx)
{
	if (_idx.x < 0 || _idx.x >= BOARD_WIDTH)
	{
		return true;
	}
	if (_idx.y < 0 || _idx.y >= BOARD_HEIGHT)
	{
		return true;
	}
	return false;
}

int board_base::countWays(vector2D<int> _idx)
{
	int cnt = 0;

	if (checkGroundFromIndex(_idx, true))
	{		
		for (int d = 0; d < Direction_MAX; d++)
		{
			vector2D<int> newIdx = { _idx.x + getDirectionSigns(d).x,_idx.y + getDirectionSigns(d).y };

			if (checkGroundFromIndex(newIdx, true))
			{
				cnt++;
			}
		}
	}
	return cnt;
}
int board_base::convertCellIdToCellVisual(vector2D<int> idx, int _cellId)
{
	SetDrawBright(255, 255, 255);

	switch (_cellId)
	{
	case CellId_Ground:
	case CellId_IceGround:
		return getGroundVisualFromIndex(idx);

	case CellId_HardBlock:
		return getHardVisualFromIndex(idx);

	case CellId_SoftBlock:
		return CellVisual_SoftBlock;

	case CellId_Pothole:
		return CellVisual_Pothole;

	case CellId_PressureBlock:
		SetDrawBright(100, 100, 100);
		return CellVisual_HardBlock_2;

	case CellId_FiredSoftBlock:
		SetDrawBright(255, 80, 0);
		return CellVisual_SoftBlock;

	default:
		break;
	}
	return CellVisual_Ground_1;
}

cellParam board_base::getCell(vector2D<int> idx)
{
	if (checkOffScreen(idx))
	{
		cellParam rv;
		rv.type = CellId_HardBlock;
		return rv;
	}
	return cells[idx.y][idx.x];
}