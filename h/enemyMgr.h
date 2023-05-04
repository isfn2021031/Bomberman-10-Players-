#pragma once
#include "../h/elementMgr.h"

class enemyMgr :public elementMgr
{
public:
	enemyMgr();
	~enemyMgr();
	element*		setNewElement		(vector2D<int> idx)	override;
	element*		getElementFromIndex	(vector2D<int> idx)	override;

private:
	static int		enemyProbability	();
};