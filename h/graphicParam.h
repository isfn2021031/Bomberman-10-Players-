#pragma once
#include <string>

using namespace std;

struct graphicParam
{
	int handle = -1;
	int width = 1;
	int height = 1;
};
graphicParam LoadGraphParam(string fileName);
void LoadDivGraphParam(string fileName, int _size, int xNum, int yNum, int xSize, int ySize, graphicParam param[]);
