#include "DxLib.h"
#include "../h/player_make.h"
#include "../h/input_key.h"
#include "../h/soundMgr.h"

int player_make::GH_Frame = -1;

player_make::player_make() :player_base(0)
{
	kI = new input_key(DX_INPUT_KEY_PAD1);

	setExistence(true);
	moveSpeed = 4;
	
	bombs.setCapacity(BOMB_MAX);

	if (GH_Frame == -1)
	{
		GH_Frame = LoadGraph("textures/Frame.png");
	}
}
player_make::~player_make()
{
	for (auto v : GH_types)
	{
		DeleteGraph(v);
	}
	GH_types.clear();
}

void player_make::Update()
{
	player_base::Update();

	if (nowSet)
	{
		int se = -1;
		int realType = type;

		if (type == CellId_Ground)
		{
			realType = -1;

			if (gimmicks != nullptr)
			{
				se = SE_BackSpace;
				gimmicks->deleteGimmick(getHitBoxCenterIndex());
			}
			element* pBomb = bombs.getElementFromIndex(getHitBoxCenterIndex());
			if (pBomb != nullptr)
			{
				pBomb->setExistence(false);
			}
		}
		if (type < CellId_MAX)
		{
			if (type < CellId_MAX)
			{
				if (pBoard != nullptr)
				{
					if (se == -1)
					{
						se = SE_BombSet;
					}
					pBoard->setCell(getHitBoxCenterIndex(), realType);
				}
			}
		}
		else if (type < (CellId_MAX + GimmickID_MAX))
		{
			if (gimmicks != nullptr)
			{
				realType = type - CellId_MAX;

				if (gimmicks->createNewGimmick(getHitBoxCenterIndex(), realType))
				{
					se = SE_BombSet;
				}
			}
		}
		else
		{
			bombStatus bs;
			bs.bombType = type - (CellId_MAX + GimmickID_MAX);
			bs.power = 2;

			if (bombs.setNewBomb(getHitBoxCenterIndex(), bs))
			{
				if (pBoard != nullptr)
				{
					pBoard->setBombID(getHitBoxCenterIndex(), bs.bombType);
					se = SE_BombSet;
				}
			}
		}
		if (se != -1)
		{
			soundMgr::GetInstance().Play(se);
		}
	}
	if (kI->getButtonEdge(actButton_R))
	{
		soundMgr::GetInstance().Play(SE_Select);
		type = ++type % (CellId_MAX + GimmickID_MAX + BombType_MAX);
	}
	if (kI->getButtonEdge(actButton_L))
	{
		soundMgr::GetInstance().Play(SE_Select);
	
		if (--type < 0)
		{
			type = (CellId_MAX + GimmickID_MAX + BombType_MAX) - 1;
		}
	}
	alphaCnt = (++alphaCnt) % 256;
}
void player_make::Draw()
{
	player_base::Draw();

	int dx = BASIS_PX + (getHitBoxCenterIndex().x * CELL_SIZE) + (CELL_SIZE / 2);
	int dy = BASIS_PY + (getHitBoxCenterIndex().y * CELL_SIZE) + (CELL_SIZE / 2);
	DrawRotaGraph(dx, dy, 1.0, 0.0, GH_Frame, TRUE);

	int blendMode = 0;
	int blendParam = 0;
	GetDrawBlendMode(&blendMode, &blendParam);
	blendParam = abs(alphaCnt - 128) + 128;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, blendParam);
	DrawRotaGraph(dx, dy, 1.0, 0.0, GH_types[type], TRUE);
	SetDrawBlendMode(blendMode, blendParam);

	bombs.Draw();
}

void player_make::setBoard(board_base* mk_b) 
{ 
	pBoard = mk_b; 
	GH_types.clear();

	for (int i = 0; i < CellId_MAX; i++)
	{
		GH_types.push_back(pBoard->getCellGH(CellStyle_Classic, board_base::convertCellIdToCellVisual({ 0,0 }, i)));
	}
	int GH_gimmicks[GimmickID_MAX] = { -1 };
	LoadDivGraph("textures/gimmicks/editor.png", GimmickID_MAX, GimmickID_MAX, 1, CELL_SIZE, CELL_SIZE, GH_gimmicks);

	for (int i = 0; i < GimmickID_MAX; i++)
	{
		GH_types.push_back(GH_gimmicks[i]);
	}
	
	int GH_bombIcon[BombType_MAX];
	LoadDivGraph("textures/bomb/icons.png", BombType_MAX, BombType_MAX, 1, 22, 23, GH_bombIcon);

	for (int i = 0; i < BombType_MAX; i++)
	{
		GH_types.push_back(GH_bombIcon[i]);
	}
}
bool player_make::checkHit(vector2D<pixelPosi> p)
{
	if (checkOffScreen(p))
	{
		return true;
	}
	return false;
}