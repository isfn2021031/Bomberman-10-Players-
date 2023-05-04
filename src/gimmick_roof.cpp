#include "DxLib.h"
#include "../h/gimmick_roof.h"
#include "../h/board_main.h"

gimmick_roof::gimmick_roof(string _FileName)
{
	GH_roof = LoadGraph(_FileName.c_str());
	Width = Height = 3;
}
gimmick_roof::~gimmick_roof()
{
	DeleteGraph(GH_roof);
}

void gimmick_roof::DrawSurface()
{
	if (getExistence())
	{
		DrawGraph(BASIS_PX + getHitBoxPixel().x, BASIS_PY + getHitBoxPixel().y - 15, GH_roof, TRUE);
	}
}

void gimmick_roof::setHitBoxIndex(vector2D<int> _idx)
{
	gimmick::setHitBoxIndex(_idx);

	board_main* pBoard = board_main::getInstance();
	if (pBoard != nullptr)
	{
		if (type == 0)
		{
			pBoard->setCell(_idx,						CellId_HardBlock);
			pBoard->setCell({ _idx.x + 2, _idx.y },		CellId_HardBlock);
			pBoard->setCell({ _idx.x, _idx.y + 2 },		CellId_HardBlock);
			pBoard->setCell({ _idx.x + 2, _idx.y + 2 }, CellId_HardBlock);

			pBoard->setCell({ _idx.x + 1, _idx.y });
			pBoard->setCell({ _idx.x, _idx.y + 1 });
			pBoard->setCell({ _idx.x + 1, _idx.y + 1 });
			pBoard->setCell({ _idx.x + 2, _idx.y + 1 });
			pBoard->setCell({ _idx.x + 1, _idx.y + 2 });
		}
	}
}