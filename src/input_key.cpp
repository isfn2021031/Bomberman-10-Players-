#include "DxLib.h"
#include "../h/input_key.h"
#include "../h/configLoader.h"
#include "../h/debug.h"

const int input_key::PadIdTable[PAD_MAX] =
{
	DX_INPUT_PAD1,	// パッド１
	DX_INPUT_PAD2,	// パッド２
	DX_INPUT_PAD3,	// パッド３
	DX_INPUT_PAD4,	// パッド４
	DX_INPUT_PAD5,
	DX_INPUT_PAD6,
	DX_INPUT_PAD7,
	DX_INPUT_PAD8,
	DX_INPUT_PAD9,
	DX_INPUT_PAD10,
	DX_INPUT_PAD11,	// パッド11
	DX_INPUT_PAD12,	// パッド12
	DX_INPUT_PAD13,	// パッド13
	DX_INPUT_PAD14,	// パッド14
	DX_INPUT_PAD15,
	DX_INPUT_PAD16,
};
const int input_key::ButtonIdTable[BUTTON_MAX] =
{
	PAD_INPUT_1,
	PAD_INPUT_2,
	PAD_INPUT_3,
	PAD_INPUT_4,
	PAD_INPUT_5,
	PAD_INPUT_6,
	PAD_INPUT_7,
	PAD_INPUT_8,
	PAD_INPUT_9,
	PAD_INPUT_10,
	PAD_INPUT_11,
	PAD_INPUT_12,
	PAD_INPUT_13,
	PAD_INPUT_14,
	PAD_INPUT_16,
	PAD_INPUT_17,
	PAD_INPUT_18,
	PAD_INPUT_19,
	PAD_INPUT_20,
	PAD_INPUT_21,
	PAD_INPUT_22,
	PAD_INPUT_23,
	PAD_INPUT_24,
	PAD_INPUT_25,
	PAD_INPUT_26,
	PAD_INPUT_27,
	PAD_INPUT_28,
};

input_key::input_key(int code, int id) :input_base(code, id)
{
	kCode = code;

	TCHAR joypadName[MAX_PATH];
	TCHAR productName[MAX_PATH];

	if (GetJoypadName(kCode, joypadName, productName) == 0)
	{
		joypadStr = joypadName;
		productStr = productName;
	}
	configLoader cl("bin/config/" + joypadStr + ".bin", actButton_MAX);

	if (cl.getActive())
	{
		for (int i = 0; i < actButton_MAX; i++)
		{
			config_actButtons[i] = cl.getConfig(i);
		}
	}
	else
	{
		config_actButtons[actButton_A] = 1;
		config_actButtons[actButton_B] = 0;
		config_actButtons[actButton_L] = 4;
		config_actButtons[actButton_R] = 5;
		config_actButtons[actButton_P] = 9;
	}
}

void input_key::Initialize()
{
	for (int b = 0; b < BUTTON_MAX; b++)
	{
		buttons[b].input = false;
		buttons[b].edgeInput = false;
	}
}
void input_key::Update()
{
	if (frameCnt > 12)
	{ 
		// キー入力取得
		int Key = GetJoypadInputState(kCode);
		GetJoypadAnalogInput(&analogX, &analogY, kCode);

		arrow_up.input = (Key & PAD_INPUT_UP);
		arrow_up.edgeInput = ((Key & ~OldKey) & PAD_INPUT_UP);

		arrow_down.input = (Key & PAD_INPUT_DOWN);
		arrow_down.edgeInput = ((Key & ~OldKey) & PAD_INPUT_DOWN);

		arrow_left.input = (Key & PAD_INPUT_LEFT);
		arrow_left.edgeInput = ((Key & ~OldKey) & PAD_INPUT_LEFT);

		arrow_right.input = (Key & PAD_INPUT_RIGHT);
		arrow_right.edgeInput = ((Key & ~OldKey) & PAD_INPUT_RIGHT);

		for (int b = 0; b < BUTTON_MAX; b++)
		{
			buttons[b].input = (Key & ButtonIdTable[b]);
			buttons[b].edgeInput= ((Key & ~OldKey) & ButtonIdTable[b]);
		}
		OldKey = Key;

		direction = createDirection();// Analog();
		directionEdge = createDirectionEdge();
		subDirection = createSubDirection();
	}
	
	frameCnt++;

	if (DebugMgr::GetInstance().getMode() != Debug_None)
	{
		DebugMgr::GetInstance().pushStr(joypadStr + ", " + productStr);
	}
}
void input_key::Draw()
{
}

void input_key::setButton(int idx, int id)
{
	if (idx >= 0 && idx < actButton_MAX)
	{
		config_actButtons[idx] = id;
	}
}

bool input_key::getButtonReal(int buttonID)
{
	if (buttonID >= 0 && buttonID < BUTTON_MAX)
	{
		return buttons[buttonID].input;
	}
	return false;
}
bool input_key::getButtonEdgeAll()
{
	for (int i = 0; i < BUTTON_MAX; i++)
	{
		if (buttons[i].edgeInput)
		{
			return true;
		}
	}
	return false;
}
bool input_key::getButtonAll()
{
	for (int i = 0; i < BUTTON_MAX; i++)
	{
		if (buttons[i].input)
		{
			return true;
		}
	}
	return false;
}
bool input_key::getButton(int idx)
{
	return getButtonReal(getConfigActButton(idx));
}
bool input_key::getButtonEdge(int idx)
{
	int button_id = getConfigActButton(idx);

	if (button_id >= 0)
	{
		return buttons[button_id].edgeInput;
	}
	return false;
}
bool input_key::getButtonDirection(int d, bool edge)
{
	switch (d)
	{
	case Direction_Down:	return edge ? arrow_down.input : arrow_down.edgeInput;
	case Direction_Up:		return edge ? arrow_up.input : arrow_up.edgeInput;
	case Direction_Right:	return edge ? arrow_right.input : arrow_right.edgeInput;
	case Direction_Left:	return edge ? arrow_left.input : arrow_left.edgeInput;
	default:				break;
	}
	return false;
}

const int input_key::getConfigActButton(int idx)
{
	if (idx >= 0 && idx < actButton_MAX)
	{
		int button_id = config_actButtons[idx];

		if (button_id >= 0)
		{
			return button_id;
		}
	}
	return -1;
}

int input_key::createDirection()
{
	int cnt = 0;

	cnt += arrow_down.input;
	cnt += arrow_up.input;
	cnt += arrow_right.input;
	cnt += arrow_left.input;

	switch (cnt)
	{
	case 1:
		direction_real = direction;
		if (arrow_down.input)
		{
			return Direction_Down;
		}
		if (arrow_up.input)
		{
			return Direction_Up;
		}
		if (arrow_right.input)
		{
			return Direction_Right;
		}
		if (arrow_left.input)
		{
			return Direction_Left;
		}
		break;

	case 2:
		return direction_real;

	default:
		break;
	}
	return Direction_None;
}
int input_key::createDirectionAnalog()
{
	int dir = abs(analogX) - abs(analogY);

	// X優勢
	if (dir > 0)
	{
		if (analogX > 0)
		{
			return Direction_Right;
		}
		if (analogX < 0)
		{
			return Direction_Left;
		}
	}
	// Y優勢
	else//if (dir < 0)
	{
		if (analogY > 0)
		{
			return Direction_Down;
		}
		if (analogY < 0)
		{
			return Direction_Up;
		}
	}
	return Direction_None;
}
int input_key::createDirectionEdge()
{
	int cnt = 0;

	cnt += arrow_down.edgeInput;
	cnt += arrow_up.edgeInput;
	cnt += arrow_right.edgeInput;
	cnt += arrow_left.edgeInput;

	if (cnt == 1)
	{
		if (arrow_down.edgeInput)
		{
			return Direction_Down;
		}
		if (arrow_up.edgeInput)
		{
			return Direction_Up;
		}
		if (arrow_right.edgeInput)
		{
			return Direction_Right;
		}
		if (arrow_left.edgeInput)
		{
			return Direction_Left;
		}
	}
	return Direction_None;
}