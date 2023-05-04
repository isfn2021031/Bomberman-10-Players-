#pragma once
#include "../h/itemPanel.h"
#include "../h/elementMgr.h"
#include "../h/configLoader.h"

class itemMgr :public elementMgr
{
public:
	itemMgr										();
	virtual				~itemMgr				();
	void				Update					()override;

	element*			setNewElement			(vector2D<int>_idx)override;
	void				Scatter					(vector<int> history);
	void				setItems_Normal			();
	void				setItems_ItemGrab		();

	static int			GetExistenceLightweight	(vector2D<int> _idx);

private:
	configLoader*		pCL = nullptr;
	static int			ExistenceOnBoard[BOARD_HEIGHT][BOARD_WIDTH];
	int					elementIdx = 0;
	int					dropRateIdx = 0;
	int					stageID = 0;
	int					styleID = 0; 
	const double		dropRates[4] = { 0.0, 0.25, 0.50, 0.75 };

	int					itemProbability();
};