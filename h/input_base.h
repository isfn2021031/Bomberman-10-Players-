#pragma once
#include "../h/global.h"

enum
{
	actButton_A,
	actButton_B,
	actButton_L,
	actButton_R,
	actButton_P,

	actButton_MAX
};

struct keyInputData
{
	bool input = false;
	bool edgeInput = false;
};

class input_base
{
public:
	input_base							(int code, int id);

	virtual void	Update				()								{}
	virtual void	Draw				()								{}
	virtual void	DrawStatus			()								{}

	void			setHitBoxCenterIdx	(vector2D<int>* centerIdx_ptr)	{ this->centerIdx_ptr = centerIdx_ptr; }
	virtual bool	getButton			(int idx = 0)					{ return false; }
	virtual bool	getButtonEdge		(int n)							{ return false; }
	int				getKCode			()								{ return kCode; };
	virtual int		getDirection		()								{ return direction; }
	virtual int		getDirectionEdge	()								{ return directionEdge; }
	int				getSubDirection		()								{ return subDirection; }

protected:
	vector2D<int>*	centerIdx_ptr = nullptr; 
	int				playerID = 0;
	int				kCode = -1;
	int				direction = Direction_None;
	int				direction_real = Direction_None;
	int				directionEdge = Direction_None;
	int				subDirection = Direction_None;

	virtual int		createSubDirection() { return Direction_None; }

private:
};