#pragma once
#include "../h/graphicParam.h"

class effect_rotaGraph3
{
public:
	effect_rotaGraph3(graphicParam _param, double _interval = 0.1);
	~effect_rotaGraph3();

	void			Update();
	void			Draw(int dx,int dy);

	graphicParam	getParam()			{ return param; }

private:
	graphicParam	param;

	bool			plus = true;
	double			rate = 0.0;
	double			interval = 0.1;
};
