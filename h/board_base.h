#pragma once
#include "../h/Task.h"

#define IDENTIFICATION	(100)

enum
{
	CellId_Ground,
	
	CellId_Pothole,
	CellId_HardBlock,
	CellId_SoftBlock,
	
	CellId_MAX,
	
	CellId_IceGround,
	CellId_FiredSoftBlock,
	CellId_PressureBlock,
};
enum
{
	CellVisual_Ground_1,
	CellVisual_Ground_2,

	CellVisual_Pothole,
	CellVisual_HardBlock_1,
	CellVisual_HardBlock_2,
	CellVisual_SoftBlock,

	CellVisual_MAX,

	CellVisual_IceGround,
	CellVisual_FiredSoftBlock,
	CellVisual_PressureBlock,
};

struct cellParam
{
	// ブロックの種類
	int				type = CellId_Ground;
	
	// 方向データ（NPCのAI用）
	int				direction = Direction_None;

	// このフラグが立っているマスにはアイテムやギミックをセットできない
	bool			dug = false;
};

class board_base :public Task
{
public:
	board_base								();
	virtual			~board_base				();
	virtual void	Update					()override;
	virtual void	Draw					()override;

	//////////////////////////////////////////////////	getter	///////////////////////////////////////////////////////////////	
	void			getCells				(int wCells[BOARD_HEIGHT][BOARD_WIDTH]);
	cellParam		getCell					(vector2D<int> idx);
	int				getCellStyle()									{ return cellStyle; }
	static int		getGroundVisualFromIndex(vector2D<int> idx)		{ return ((idx.x % 2 == 0 && idx.y % 2 == 0) || ((idx.x + 1) % 2 == 0 && (idx.y + 1) % 2 == 0)) ? CellVisual_Ground_2 : CellVisual_Ground_1; }
	static int		getHardVisualFromIndex	(vector2D<int> idx)		{ return ((idx.x > BOARD_SHIFT_X) && (idx.x < BOARD_SHIFT_X + BOARD_WD - 1) && (idx.y > BOARD_SHIFT_Y) && (idx.y < BOARD_SHIFT_Y + BOARD_HD - 1)) ? CellVisual_HardBlock_1 : CellVisual_HardBlock_2; }
	static int		getCellGH				(int style, int visual)	{ return GH_cells[style][visual]; }
	static int		convertCellIdToCellVisual(vector2D<int> idx, int _cell);

	//////////////////////////////////////////////////	setter	///////////////////////////////////////////////////////////////	
	void			setCellStyleGround		(int _cellStyle);
	void			setCell					(vector2D<int> idx, int cell = -1);
	void			setDirection			(vector2D<int> idx, int direction);
	virtual void	setBombID				(vector2D<int> _idx, int id) {};

	static bool		checkOffScreen			(vector2D<int> _idx);
	static bool		checkGround				(int _cell);
	virtual bool	checkGroundFromIndex	(vector2D<int>_idx, bool withBomb = false);
	int				countWays				(vector2D<int> _idx);

protected:
	cellParam		cells[BOARD_HEIGHT][BOARD_WIDTH];
	int				cellStyle = CellStyle_Classic;
	int				groundId = CellId_Ground;

	void			clearCells				();
	void			fireAllSoftBlocks		();
	void			setGround				();
	void			setWall					();
	void			setHardBlocks			(int _cellId = CellId_HardBlock);
	
private:
	static int		GH_cells[CellStyle_MAX][CellVisual_MAX];
};