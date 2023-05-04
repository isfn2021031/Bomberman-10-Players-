#pragma once
#include "../h/element_accessableBoard.h"
#include "../h/board_base.h"

class gimmick :public element_accessableBoard
{
public:
					gimmick			();
	virtual			~gimmick		();
	virtual void	Update			();
	virtual void	Draw			()				{}
	virtual void	DrawSurface		()				{}

	//////////////////////////////////////////////////	setter	///////////////////////////////////////////////////////////////	
	void			setDirection	(int d)override;
	virtual void	setBoardPtr		(board_base* p)	{ pBoard = p; }
	void			setWidth		(int _width)	{ Width = _width; }
	void			setHeight		(int _height)	{ Height = _height; }
	void			setSwitchPointer(bool* p)		{ pReverse = p; }

	virtual bool	getHitPlayer	(int id)		{ return hitPlayers[id]; }
	bool			getDestruction	()				{ return destruction; }
	int				getWidth		()				{ return Width; }
	int				getHeight		()				{ return Height; }
	
protected:
	board_base*		pBoard = nullptr;
	bool*			pReverse = nullptr;
	bool			destruction = true;				// プレイヤーのスタート地点に出現した場合に削除するためのフラグ
	bool			hitPl = false;
	bool			hitPl_old = false;
	bool			hitPlayers[PLAYER_MAX] = { 0 };

	int				Width = 1;
	int				Height = 1;
	int				direction_real = Direction_None;
};