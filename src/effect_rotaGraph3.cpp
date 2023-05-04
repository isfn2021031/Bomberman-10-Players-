#include "DxLib.h"
#include "../h/effect_rotaGraph3.h"

effect_rotaGraph3::effect_rotaGraph3(graphicParam _param, double _interval)
{
	param = _param;
	interval = _interval;
}
effect_rotaGraph3::~effect_rotaGraph3()
{

}
void effect_rotaGraph3::Update()
{
	rate += interval * (plus ? 1 : -1);

	if ((rate > 1.0) || (rate < -1.0))
	{
		plus = !plus;
	}
}
void effect_rotaGraph3::Draw(int dx,int dy)
{
	DrawRotaGraph3(dx, dy, param.width * 0.5, param.height * 0.5, rate, 1.0, 0.0, param.handle, TRUE);
}