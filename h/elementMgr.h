#pragma once
#include "../h/element.h"
#include "../h/graphicMgr_bomberman.h"

#define ELEMENT_MAX		(100)

class elementMgr
{
public:
	elementMgr										();
	virtual						~elementMgr			();

	virtual void				Update				();
	void						Draw				();

	static void					DrawPlayersPreview	(int pattern);

	void						setRestoration		(bool f)				{ restoration = f; }
	virtual void				setPlayerID			(int id);
	void						setCapacity			(int n);

	virtual element*			setNewElement		(vector2D<int>_idx);
	virtual element*			getElementFromIndex	(vector2D<int> idx);
	bool						setExistenceBoard	(vector2D<int> idx);
	bool						getExistenceBoard	(vector2D<int> idx);
	int							getCapacity			()						{ return capacity; }
	int							getExistenceNum		()						{ return existenceNum; }
	static const vector2D<int>	StartPositions[StartPattern_MAX][PLAYER_MAX];

protected:
	element*					elements[ELEMENT_MAX] = { nullptr };
	bool						existenceBoard[BOARD_HEIGHT][BOARD_WIDTH] = { false }; 
	bool						restoration = false;
	int							playerID = 0;
	int							capacityMAX = ELEMENT_MAX;
	int							capacity = 1;
	int							existenceNum = 0;
	int							lastSetIndex = 0;

	static graphicMgr_bomberman* bomberGraph[PLAYER_MAX];

private:
	void						clearExistenceBoard(); 
};