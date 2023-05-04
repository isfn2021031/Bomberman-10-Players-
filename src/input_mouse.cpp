#include "DxLib.h"
#include "../h/input_mouse.h"

int input_mouse::MouseInputList[MOUSE_INPUT_MAX] =
{
	MOUSE_INPUT_1,
	MOUSE_INPUT_2,
	MOUSE_INPUT_3,
	MOUSE_INPUT_4,
	MOUSE_INPUT_5,
	MOUSE_INPUT_6,
	MOUSE_INPUT_7,
	MOUSE_INPUT_8,
};

input_mouse::input_mouse(int code, int id) :input_base(code, id)
{
	kCode = code;
}

void input_mouse::Update()
{
	GetMousePoint(&mouseX, &mouseY);

	if (FrameCounter > 12)
	{ 		
		int Mouse = GetMouseInput();

		for (int b = 0; b < MOUSE_INPUT_MAX; b++)
		{
			buttons[b] = (Mouse & MouseInputList[b]);
			edgeButtons[b] = ((Mouse & ~OldMouse) & MouseInputList[b]);
		}
		direction = createDirection();
		
		OldMouse = Mouse;

		OldWheelCnt = wheelCnt;
		wheelCnt = GetMouseWheelRotVol();
	}

	if (getButtonEdge(2))
	{
		//stop = !stop;
	}
	FrameCounter++;
}
void input_mouse::Draw()
{
}

bool input_mouse::getButton(int _idx)
{
	switch (_idx)
	{
	case actButton_L:		return buttons[2];
	case actButton_R:		return buttons[1];
	case actButton_A:		return buttons[0];
	case actButton_B:		return false;
	default:	break;
	}
	return false;
}
bool input_mouse::getButtonEdge(int _idx)
{
	switch (_idx)
	{
	case actButton_L:		return edgeButtons[2];
	case actButton_R:		return edgeButtons[1];
	case actButton_A:		return edgeButtons[0];
	case actButton_B:		return wheelCnt != 0 && wheelCnt != OldWheelCnt;
	default:	break;
	}
	return false;
}

int input_mouse::createDirection()
{
	if (stop)
	{
		return Direction_None;
	}
	if (centerIdx_ptr != nullptr)
	{
		int px = (centerIdx_ptr->x * CELL_SIZE) + BASIS_PX + CELL_SIZE / 2;
		int py = (centerIdx_ptr->y * CELL_SIZE) + BASIS_PY + CELL_SIZE / 2;

		int moveX = mouseX - px;
		int moveY = mouseY - py;

		if ((abs(moveX) == abs(moveY)))
		{
			return Direction_None;
		}

		if (abs(moveX) > abs(moveY))
		{
			if (abs(moveX) >= (CELL_SIZE / 2))
			{
				return (moveX < 0) ? Direction_Left : Direction_Right;
			}
		}
		else
		{
			if (abs(moveY) >= (CELL_SIZE / 2))
			{
				return (moveY < 0) ? Direction_Up : Direction_Down;
			}
		}
	}
	return Direction_None;
}