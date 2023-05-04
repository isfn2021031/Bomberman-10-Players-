#pragma once
#include "../h/gimmick.h"

#define CONVEYOR_GRAPH_MAX		(4)

class gimmick_conveyor :public gimmick
{
public:
	gimmick_conveyor();

	void Update();
	void Draw();

private:
	static int GH_conveyors[Direction_MAX][CONVEYOR_GRAPH_MAX];
};