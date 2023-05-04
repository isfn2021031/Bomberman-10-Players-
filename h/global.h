#pragma once
#include <string>

using namespace std;

#define PLAYER_MAX				(10)

#define WINDOW_WIDTH			(1120)
#define WINDOW_HEIGHT			(630)

#define BOARD_SHIFT_X			(2)
#define BOARD_SHIFT_Y			(1)
#define BOARD_WD				(19)
#define BOARD_HD				(13)
#define BOARD_REAL_WIDTH		(BOARD_WD - 2)
#define BOARD_REAL_HEIGHT		(BOARD_HD - 2)
#define BOARD_WIDTH				(BOARD_WD + (BOARD_SHIFT_X * 2))
#define BOARD_HEIGHT			(BOARD_HD + (BOARD_SHIFT_Y * 2))

#define CELL_SIZE				(45)

#define BASIS_PX				((pixelPosi)(CELL_SIZE * 1.00))
#define BASIS_PY				((pixelPosi)(-(CELL_SIZE * 0.175)))

enum
{
	Direction_Down,
	Direction_Up,
	Direction_Left,
	Direction_Right,

	Direction_MAX,
	Direction_None,
};

enum
{
	CellStyle_Classic,
	CellStyle_Retrospective,
	CellStyle_Desert,
	CellStyle_SlipSliding,
	CellStyle_Volcano,
	CellStyle_Horror,

	CellStyle_Speedy,
	CellStyle_Factory,
	CellStyle_PowerFreaks,

	CellStyle_MAX,
};
enum
{
	DiagonalDirection_UL,
	DiagonalDirection_UR,
	DiagonalDirection_DL,
	DiagonalDirection_DR,

	DiagonalDirection_MAX,
	DiagonalDirection_None,
};
enum
{
	plGraph_Idle,
	plGraph_WalkR,
	plGraph_WalkL,

	plGraph_MAX
};
enum
{
	posi_Main,
	posi_subRU,
	posi_subRD,
	posi_subLD,

	posi_MAX
};
enum
{
	Team_Blue,
	Team_Red,
	Team_Green,
	Team_Yellow,
	Team_Pink,
	Team_SkyBlue,
	Team_Orange,
	Team_Purple,
	Team_None,

	Team_MAX,
};
enum
{
	StartPattern_Normal,
	StartPattern_Surrounding,
	StartPattern_ItemsGrab,
	StartPattern_UpDown,
	StartPattern_GroupCenter,
	StartPattern_GroupUp,
	StartPattern_GroupDown,
	StartPattern_Crown,

	StartPattern_MAX
};

typedef int pixelPosi;

template<class V>
class vector2D
{
public:
	V x;
	V y;

	bool operator ==(const vector2D& r)
	{
		return this->x == r.x && this->y == r.y;
	}
	bool operator !=(const vector2D& r)
	{
		return this->x != r.x || this->y != r.y;
	}
	vector2D operator +(const vector2D& r)
	{
		vector2D rv;
		rv.x = this->x + r.x;
		rv.y = this->y + r.y;
		return rv;
	}
	vector2D operator -(const vector2D& r)
	{
		vector2D rv;
		rv.x = this->x - r.x;
		rv.y = this->y - r.y;
		return rv;
	}
	vector2D operator *(const vector2D& r)
	{
		vector2D rv;
		rv.x = this->x * r.x;
		rv.y = this->y * r.y;
		return rv;
	}
	vector2D operator /(const vector2D& r)
	{
		vector2D rv;
		rv.x = this->x / r.x;
		rv.y = this->y / r.y;
		return rv;
	}
};

vector2D<int>			getDirectionSigns	(int d);
vector2D<int>			getDirctionSigns2	(int d);
string					getDirectionString	(int d);
int						getReverseDirection	(int d);