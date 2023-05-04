#include "DxLib.h"
#include "../h/effect_fade.h"
#include "../h/global.h"

effect_fade::effect_fade(bool out, int n)
{
	rate = out ? 255 : 0;
	sign = n * (out ? -1 : 1);
}
effect_fade::~effect_fade()
{
}
int effect_fade::Update()
{
	rate += sign;

	if (rate > 255)
	{
		return 1;
	}
	else if (rate < 0)
	{
		return - 1;
	}
	return 0;
}
void effect_fade::Draw()
{
	int BlendMode = 0;
	int BlendParam = 0;
	GetDrawBlendMode(&BlendMode, &BlendParam);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, rate);
	DrawBox(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(BlendMode, BlendParam);
}