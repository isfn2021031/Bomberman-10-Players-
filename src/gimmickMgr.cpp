#include <stdlib.h>
#include <vector>
#include "DxLib.h"
#include "../h/myfunc.h"
#include "../h/gimmickMgr.h"
#include "../h/gimmick_conveyor.h"
#include "../h/gimmick_kickPanel.h"
#include "../h/gimmick_roof.h"
#include "../h/gimmick_switch.h"
#include "../h/gimmick_warpPoint.h"
#include "../h/gimmick_enemyPoint.h"

gimmickMgr::gimmickMgr(board_base* p)
{
	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			used[y][x] = false;
			gimmickIDs[y][x] = GimmickID_None;
			gimmicks[y][x] = new gimmick;
		}
	}
	pBoard = p;
	reverse = (rand() % 2);
	gimmick_warpPoint::Initialize();

}
gimmickMgr::~gimmickMgr()
{
	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			if (gimmicks[y][x] != nullptr)
			{
				delete gimmicks[y][x];
			}
		}
	}
}

void gimmickMgr::Initialize()
{
	gimmick_warpPoint::CreateWarpDestination();
}
void gimmickMgr::Update()
{
	bool reverse_old = reverse;

	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			if (used[y][x] && gimmicks[y][x] != nullptr)
			{
				gimmicks[y][x]->Update();
			}
		}
	}
	if (reverse != reverse_old)
	{
		for (int y = 0; y < BOARD_HEIGHT; y++)
		{
			for (int x = 0; x < BOARD_WIDTH; x++)
			{
				if (used[y][x] && gimmicks[y][x] != nullptr)
				{
					int nowDir = gimmicks[y][x]->getDirection();

					if (nowDir != Direction_None)
					{
						int newDir = getReverseDirection(nowDir);
						gimmicks[y][x]->setDirection(newDir);
					}
				}
			}
		}

	}
}
void gimmickMgr::Draw()
{
	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			if (gimmicks[y][x] != nullptr)
			{
				if (used[y][x])
				{
					gimmicks[y][x]->Draw();
				}
			}
		}
	}
}

void gimmickMgr::DrawSurfaces()
{
	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			if (used[y][x] && gimmicks[y][x] != nullptr)
			{
				gimmicks[y][x]->DrawSurface();
			}
		}
	}
}
void gimmickMgr::DrawUsedAreas()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);

	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			if (used[y][x])
			{
				int dx = BASIS_PX + x * CELL_SIZE;
				int dy = BASIS_PY + y * CELL_SIZE;

				DrawBox(dx, dy, dx + CELL_SIZE, dy + CELL_SIZE, GetColor(250, 70, 50), TRUE);
			}
		}
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

// 新しいギミック（インスタンス）生成
bool gimmickMgr::createNewGimmick(vector2D<int> _idx, int gimmickType)
{
	if (gimmickIDs[_idx.y][_idx.x] == GimmickID_None)
	{
		gimmickIDs[_idx.y][_idx.x] = gimmickType;

		const vector2D<int> size = getSize(gimmickType);

		for (int y = 0; y < size.y; y++)
		{
			for (int x = 0; x < size.x; x++)
			{
				vector2D<int> newPosi = { _idx.x + x ,_idx.y + y };

				if (element::checkOffScreenIdx(newPosi))
				{
					return false;
				}
				if (used[newPosi.y][newPosi.x])
				{
					return false;
				}
				used[_idx.y + y][_idx.x + x] = true;
			}
		}
		switch (gimmickType)
		{
		case GimmickID_Conveyor_D:
			gimmicks[_idx.y][_idx.x] = new gimmick_conveyor;
			gimmicks[_idx.y][_idx.x]->setHitBoxIndex(_idx);
			gimmicks[_idx.y][_idx.x]->setDirection(Direction_Down);
			gimmicks[_idx.y][_idx.x]->setType(GimmickType_BeltConveyor);
			break;

		case GimmickID_Conveyor_U:
			gimmicks[_idx.y][_idx.x] = new gimmick_conveyor;
			gimmicks[_idx.y][_idx.x]->setHitBoxIndex(_idx);
			gimmicks[_idx.y][_idx.x]->setDirection(Direction_Up);
			gimmicks[_idx.y][_idx.x]->setType(GimmickType_BeltConveyor);
			break;

		case GimmickID_Conveyor_L:
			gimmicks[_idx.y][_idx.x] = new gimmick_conveyor;
			gimmicks[_idx.y][_idx.x]->setHitBoxIndex(_idx);
			gimmicks[_idx.y][_idx.x]->setDirection(Direction_Left);
			gimmicks[_idx.y][_idx.x]->setType(GimmickType_BeltConveyor);
			break;

		case GimmickID_Conveyor_R:
			gimmicks[_idx.y][_idx.x] = new gimmick_conveyor;
			gimmicks[_idx.y][_idx.x]->setHitBoxIndex(_idx);
			gimmicks[_idx.y][_idx.x]->setDirection(Direction_Right);
			gimmicks[_idx.y][_idx.x]->setType(GimmickType_BeltConveyor);
			break;

		case GimmickID_Kickpanel_D:
			gimmicks[_idx.y][_idx.x] = new gimmick_kickPanel;
			gimmicks[_idx.y][_idx.x]->setHitBoxIndex(_idx);
			gimmicks[_idx.y][_idx.x]->setDirection(Direction_Down);
			gimmicks[_idx.y][_idx.x]->setType(GimmickType_KickPanel);
			break;

		case GimmickID_Kickpanel_U:
			gimmicks[_idx.y][_idx.x] = new gimmick_kickPanel;
			gimmicks[_idx.y][_idx.x]->setHitBoxIndex(_idx);
			gimmicks[_idx.y][_idx.x]->setDirection(Direction_Up);
			gimmicks[_idx.y][_idx.x]->setType(GimmickType_KickPanel);
			break;

		case GimmickID_Kickpanel_L:
			gimmicks[_idx.y][_idx.x] = new gimmick_kickPanel;
			gimmicks[_idx.y][_idx.x]->setHitBoxIndex(_idx);
			gimmicks[_idx.y][_idx.x]->setDirection(Direction_Left);
			gimmicks[_idx.y][_idx.x]->setType(GimmickType_KickPanel);
			break;

		case GimmickID_Kickpanel_R:
			gimmicks[_idx.y][_idx.x] = new gimmick_kickPanel;
			gimmicks[_idx.y][_idx.x]->setHitBoxIndex(_idx);
			gimmicks[_idx.y][_idx.x]->setDirection(Direction_Right);
			gimmicks[_idx.y][_idx.x]->setType(GimmickType_KickPanel);
			break;

		case GimmickID_Switch:
			gimmicks[_idx.y][_idx.x] = new gimmick_switch;
			gimmicks[_idx.y][_idx.x]->setHitBoxIndex(_idx);
			gimmicks[_idx.y][_idx.x]->setType(GimmickType_Switch);
			break;

		case GimmickID_Roof_Pipe:
			gimmicks[_idx.y][_idx.x] = new gimmick_roof("textures/gimmicks/EarthenPipe.png");
			gimmicks[_idx.y][_idx.x]->setHitBoxIndex(_idx);
			gimmicks[_idx.y][_idx.x]->setType(GimmickType_Roof);
			break;

		case GimmickID_Roof_Parasol:
			gimmicks[_idx.y][_idx.x] = new gimmick_roof("textures/gimmicks/Parasol.png");
			gimmicks[_idx.y][_idx.x]->setHitBoxIndex(_idx);
			gimmicks[_idx.y][_idx.x]->setType(GimmickType_Roof);
			break;

		case GimmickID_WarpPoint:
			warpPointX = _idx.x;
			warpPointY = _idx.y;
			gimmicks[_idx.y][_idx.x] = new gimmick_warpPoint;
			gimmicks[_idx.y][_idx.x]->setHitBoxIndex(_idx);
			gimmicks[_idx.y][_idx.x]->setType(GimmickType_WarpPoint);
			break;

		case GimmickID_EnemyPoint:
			gimmicks[_idx.y][_idx.x] = new gimmick_enemyPoint;
			gimmicks[_idx.y][_idx.x]->setHitBoxIndex(_idx);
			gimmicks[_idx.y][_idx.x]->setType(GimmickType_EnemyPoint);
			break;

		default:
			return false;
		}
		gimmicks[_idx.y][_idx.x]->setExistence(true);
		gimmicks[_idx.y][_idx.x]->setWidth(size.x);
		gimmicks[_idx.y][_idx.x]->setHeight(size.y);
		gimmicks[_idx.y][_idx.x]->setBoardPtr(pBoard);
		gimmicks[_idx.y][_idx.x]->setSwitchPointer(&reverse);

		return true;
	}
	return false;
}

void gimmickMgr::deleteGimmick(vector2D<int> _idx)
{
	if (gimmicks[_idx.y][_idx.x] != nullptr)
	{
		for (int y = 0; y < gimmicks[_idx.y][_idx.x]->getHeight(); y++)
		{
			for (int x = 0; x < gimmicks[_idx.y][_idx.x]->getWidth(); x++)
			{
				vector2D<int> newPosi = { _idx.x + x ,_idx.y + y };

				if (element::checkOffScreenIdx(newPosi))
				{
					continue;
				}
				used[newPosi.y][newPosi.x] = false;
			}
		}

		delete gimmicks[_idx.y][_idx.x];
		gimmicks[_idx.y][_idx.x] = nullptr;
		gimmickIDs[_idx.y][_idx.x] = GimmickID_None;
	}
}

vector2D<int> gimmickMgr::getSize(int _gimmickType)
{
	switch (_gimmickType)
	{
	case GimmickID_Conveyor_D:
	case GimmickID_Conveyor_U:
	case GimmickID_Conveyor_L:
	case GimmickID_Conveyor_R:
	case GimmickID_Kickpanel_D:
	case GimmickID_Kickpanel_U:
	case GimmickID_Kickpanel_L:
	case GimmickID_Kickpanel_R:
	case GimmickID_Switch:
	case GimmickID_WarpPoint:
	case GimmickID_EnemyPoint:
		return { 1,1 };

	case GimmickID_Roof_Pipe:
	case GimmickID_Roof_Parasol:
		return { 3,3 };

	default:
		break;
	}
	return { 0,0 };
}

void gimmickMgr::createEarthenPipes()
{
	createNewGimmick({ BOARD_SHIFT_X + 2, BOARD_SHIFT_Y + 2 }, GimmickID_Roof_Pipe);
	createNewGimmick({ BOARD_SHIFT_X + 2, BOARD_SHIFT_Y + 8 }, GimmickID_Roof_Pipe);
	createNewGimmick({ BOARD_SHIFT_X + 14, BOARD_SHIFT_Y + 2 }, GimmickID_Roof_Pipe);
	createNewGimmick({ BOARD_SHIFT_X + 14, BOARD_SHIFT_Y + 8 }, GimmickID_Roof_Pipe);
}
void gimmickMgr::createKickPanels()
{
	bool reverse = false;
	createNewGimmick({ BOARD_SHIFT_X + 1,					BOARD_SHIFT_Y + 1 },				reverse ? GimmickID_Kickpanel_D :	GimmickID_Kickpanel_R);
	createNewGimmick({ BOARD_SHIFT_X + 1,					BOARD_HEIGHT - BOARD_SHIFT_Y - 2 }, reverse ? GimmickID_Kickpanel_R :	GimmickID_Kickpanel_U);
	createNewGimmick({ BOARD_WIDTH - BOARD_SHIFT_X - 2 ,	BOARD_SHIFT_Y + 1 },				reverse ? GimmickID_Kickpanel_L :	GimmickID_Kickpanel_D);
	createNewGimmick({ BOARD_WIDTH - BOARD_SHIFT_X - 2 ,	BOARD_HEIGHT - BOARD_SHIFT_Y - 2 }, reverse ? GimmickID_Kickpanel_U :		GimmickID_Kickpanel_L);

	createNewGimmick({ BOARD_SHIFT_X + 3,					BOARD_SHIFT_Y + 3 },				reverse ? GimmickID_Kickpanel_R :	GimmickID_Kickpanel_D);
	createNewGimmick({ BOARD_SHIFT_X + 3,					BOARD_HEIGHT - BOARD_SHIFT_Y - 4 }, reverse ? GimmickID_Kickpanel_U:		GimmickID_Kickpanel_R);
	createNewGimmick({ BOARD_WIDTH - BOARD_SHIFT_X - 4 ,	BOARD_SHIFT_Y + 3 },				reverse ? GimmickID_Kickpanel_D :	GimmickID_Kickpanel_L);
	createNewGimmick({ BOARD_WIDTH - BOARD_SHIFT_X - 4 ,	BOARD_HEIGHT - BOARD_SHIFT_Y - 4 }, reverse ? GimmickID_Kickpanel_L :	GimmickID_Kickpanel_U);
}

// ベルトコンベアステージ
void gimmickMgr::createBeltZone()
{
	createBeltCircle(3, reverse);
	createBeltCircle(5, !reverse);
}
void gimmickMgr::createBeltCircle(int space, bool _reverse)
{
	const int CornerMinX = BOARD_SHIFT_X + space;
	const int CornerMinY = BOARD_SHIFT_Y + space;
	const int BeltWidth = BOARD_WD - (space * 2);
	const int BeltHeight = BOARD_HD - (space * 2);
	const int CornerMaxX = CornerMinX + BeltWidth - 1;
	const int CornerMaxY = CornerMinY + BeltHeight - 1;

	for (int y = CornerMinY + _reverse; y < CornerMinY + BeltHeight - !_reverse; y++)
	{
		createNewGimmick({ CornerMinX,y }, !_reverse ? GimmickID_Conveyor_D : GimmickID_Conveyor_U);
	}
	for (int x = CornerMinX + _reverse; x < CornerMinX + BeltWidth - !_reverse; x++)
	{
		createNewGimmick({ x,CornerMaxY }, !_reverse ? GimmickID_Conveyor_R : GimmickID_Conveyor_L);
	}
	for (int y = CornerMinY + !_reverse; y < CornerMinY + BeltHeight - _reverse; y++)
	{
		createNewGimmick({ CornerMaxX,y }, !_reverse ? GimmickID_Conveyor_U : GimmickID_Conveyor_D);
	}
	for (int x = CornerMinX + !_reverse; x < CornerMinX + BeltWidth - _reverse; x++)
	{
		createNewGimmick({ x,CornerMinY }, !_reverse ? GimmickID_Conveyor_L : GimmickID_Conveyor_R);
	}
}
void gimmickMgr::createBeltLine(int y, bool _reverse)
{
	for (int x = 0; x < BOARD_WIDTH; x++)
	{
		createNewGimmick({ x, y }, _reverse ? GimmickID_Conveyor_R : GimmickID_Conveyor_L);
	}
}

int	gimmickMgr::convertIdToType(int id)
{
	switch (id)
	{
	case GimmickID_Conveyor_D:
	case GimmickID_Conveyor_U:
	case GimmickID_Conveyor_L:
	case GimmickID_Conveyor_R:
		return GimmickType_BeltConveyor;

	case GimmickID_Kickpanel_D:
	case GimmickID_Kickpanel_U:
	case GimmickID_Kickpanel_L:
	case GimmickID_Kickpanel_R:
		return GimmickType_KickPanel;

	case GimmickID_Switch:
		return GimmickType_Switch;
	
	case GimmickID_Roof_Pipe:
	case GimmickID_Roof_Parasol:
		return GimmickType_Roof;

	case GimmickID_WarpPoint:
		return GimmickType_WarpPoint;

	case GimmickID_EnemyPoint:
		return GimmickType_EnemyPoint;
	}
	return GimmickType_None;
}
