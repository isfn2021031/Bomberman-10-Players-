#pragma once
#include "../h/input_base.h"

#define MOUSE_INPUT_MAX		(8)

class input_mouse :public input_base
{
public:
	input_mouse					(int code, int id = 0);

	void		Update			();
	void		Draw			();

	bool		getButton		(int idx = 0)override;
	bool		getButtonEdge	(int idx = 0)override;

private:
	static int	MouseInputList[MOUSE_INPUT_MAX];

	int			OldMouse = 0;
	int			mouseX = 0;
	int			mouseY = 0;
	int			oldX = 0;
	int			oldY = 0;
	int			FrameCounter = 0;
	int			wheelCnt = 0;
	int			OldWheelCnt = 0;

	bool		stop = false;
	bool		buttons[MOUSE_INPUT_MAX] = { 0 };
	bool		edgeButtons[MOUSE_INPUT_MAX] = { 0 };

	int			createDirection();
};