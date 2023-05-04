#pragma once
#include "../h/bombBlast.h"
#include "../h/elementMgr.h"

class bombBlastMgr :public elementMgr
{
public:
				bombBlastMgr	();
				~bombBlastMgr	();
	void		Update			();
	void		Draw			();

	void		setGraphIdx		(vector2D<int> _idx, int gIdx) { explosion[_idx.y][_idx.x].setGraphIdx(gIdx); }
	void		setFx			(vector2D<int> _idx, blastStatus _fx) { explosion[_idx.y][_idx.x].setForecastExplosion(_fx); }
	bombBlast*	getBlast		(vector2D<int> _idx);
	
private:
	bombBlast	explosion[BOARD_HEIGHT][BOARD_WIDTH];
};