#pragma once
#include "../h/element.h"

class element_accessableBoard :public element
{
public:
	element_accessableBoard												()	{}
	virtual								~element_accessableBoard		()	{}
	
protected:
	void								updateNeighbor					();
	virtual bool						checkHit						(vector2D<pixelPosi> p);
	bool								checkHitCell					(vector2D<pixelPosi> p, int cellType);
	virtual bool						checkHitBlast					(vector2D<int> idx);

	virtual bool						checkHitBomb					(vector2D<pixelPosi> p, bool useIgnore = false);
	bool								checkHitPlayerCenterIndex		(int _playerID);
	bool								checkMatchPlayerIndex			(int _playerID, vector2D<int> _idx);
	virtual bool						checkMatchBombIndex				(vector2D<int> _idx);
	int									calcDirectionFrom2Index			(vector2D<pixelPosi> idx1, vector2D<pixelPosi> idx2);
	pixelPosi							calcDistancePlayerPixel			(int _playerID, vector2D<pixelPosi> px1);

	bool								softBlockPass = false;
	bool								ignore[BOARD_HEIGHT][BOARD_WIDTH] = { false };	// “–‚½‚è”»’è–³Ž‹
	bool								neighborGround[Direction_MAX] = { 0 };
};