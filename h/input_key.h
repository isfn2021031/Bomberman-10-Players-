#pragma once
#include "../h/input_base.h"

#define PAD_MAX			(16)
#define BUTTON_MAX		(28)

class input_key :public input_base
{
public:
	input_key									(int code, int id = 0);
	void				Initialize				();
	void				Update					();
	void				Draw					();
		
	void				setButton				(int idx, int id);
	void				setFrameCnt				(int n)				{ frameCnt = n; }

	bool				getButtonReal			(int buttonID);
	bool				getButtonAll			();
	bool				getButtonEdgeAll		();
	bool				getButton				(int idx = 0)override;
	bool				getButtonEdge			(int idx = 0)override;
	bool				getButtonDirection		(int d, bool edge = false);

	static int			getPadID				(int id)			{ return PadIdTable[id]; }
	static int			getButtonID				(int n)				{ return ButtonIdTable[n]; }
	
	int					createDirection			();
	int					createDirectionAnalog	();
	int					createDirectionEdge		();

protected:
	keyInputData		arrow_up;
	keyInputData		arrow_down;
	keyInputData		arrow_right;
	keyInputData		arrow_left;
	keyInputData		buttons[BUTTON_MAX];
	
	int					OldKey = 0;
	int					analogX = 0;
	int					analogY = 0;
	int					frameCnt = 0;

	const int			getConfigActButton(int);

private:
	static const int	PadIdTable[PAD_MAX];
	static const int	ButtonIdTable[BUTTON_MAX];
	string				joypadStr;
	string				productStr;
	int					config_actButtons[actButton_MAX] = { -1 };
};