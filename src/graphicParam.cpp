#include "DxLib.h"
#include "../h/graphicParam.h"

graphicParam LoadGraphParam(string fileName)
{
	/*int flag = GetUseASyncLoadFlag();
	SetUseASyncLoadFlag(FALSE);
	*/
	graphicParam param;
	param.handle = LoadGraph(fileName.c_str());
	while (CheckHandleASyncLoad(param.handle) == TRUE && ProcessMessage() == 0) {}
	GetGraphSize(param.handle, &param.width, &param.height);
	//SetUseASyncLoadFlag(flag);
	return param;
}
void LoadDivGraphParam(string fileName, int _size, int xNum, int yNum, int xSize, int ySize, graphicParam param[])
{
	/*int flag = GetUseASyncLoadFlag();
	SetUseASyncLoadFlag(FALSE);
	*/
	int* tmpGH = new int[_size];
	LoadDivGraph(fileName.c_str(), _size, xNum, yNum, xSize, ySize, tmpGH);

	for (int i = 0; i < _size; i++)
	{
		while (CheckHandleASyncLoad(param[i].handle) == TRUE && ProcessMessage() == 0) {}
		param[i].handle = tmpGH[i];
		param[i].width = xSize;
		param[i].height = ySize;
	}
	//SetUseASyncLoadFlag(flag);
}