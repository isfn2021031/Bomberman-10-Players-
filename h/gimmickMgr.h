#pragma once
#include "../h/gimmick.h"

enum
{
	GimmickType_None,

	GimmickType_BeltConveyor,
	GimmickType_KickPanel,
	GimmickType_Roof,
	GimmickType_Switch,
	GimmickType_WarpPoint,
	GimmickType_EnemyPoint,
	
	GimmickType_MAX,
};
enum
{
	GimmickID_Conveyor_D,
	GimmickID_Conveyor_U,
	GimmickID_Conveyor_L,
	GimmickID_Conveyor_R,

	GimmickID_Kickpanel_D,
	GimmickID_Kickpanel_U,
	GimmickID_Kickpanel_L,
	GimmickID_Kickpanel_R,

	GimmickID_Switch,
	GimmickID_Roof_Pipe,
	GimmickID_Roof_Parasol,
	GimmickID_WarpPoint,
	GimmickID_EnemyPoint,
	
	GimmickID_MAX,
	GimmickID_None,
};


class gimmickMgr
{
public:
	gimmickMgr							(board_base* p);
	virtual			~gimmickMgr			();
	
	static void		Initialize			();
	void			Update				();
	void			Draw				();

	void			DrawSurfaces		();
	void			DrawUsedAreas		();

	void			createKickPanels	();
	void			createBeltZone		();
	void			createBeltLine		(int y, bool _reverse);
	void			createEarthenPipes	();
	void			deleteGimmick		(vector2D<int> _idx); 

	bool			createNewGimmick	(vector2D<int> _idx, int gimmickType);
	bool			getUsed				(vector2D<int> _idx) { return used[_idx.y][_idx.x]; }
	int				getID				(vector2D<int> _idx) { return gimmickIDs[_idx.y][_idx.x]; }
	int				getType				(vector2D<int> _idx) { return convertIdToType(gimmickIDs[_idx.y][_idx.x]); }

	gimmick*		getGimmick			(vector2D<int> _idx) { return gimmicks[_idx.y][_idx.x]; }

private:
	gimmick*		gimmicks[BOARD_HEIGHT][BOARD_WIDTH] = { nullptr };
	board_base*		pBoard = nullptr;
	
	bool			reverse = true;
	bool			used[BOARD_HEIGHT][BOARD_WIDTH] = { false };

	int				gimmickIDs[BOARD_HEIGHT][BOARD_WIDTH] = { GimmickID_None };
	int				warpPointX = 0;
	int				warpPointY = 0;

	vector2D<int>	getSize(int gimmickType);
	void			createBeltCircle(int space, bool _reverse);
	static int		convertIdToType(int id);
};

