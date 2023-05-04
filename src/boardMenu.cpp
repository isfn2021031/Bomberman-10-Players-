#include "DxLib.h"
#include "../h/boardMenu.h"
#include "../h/soundMgr.h"

int boardMenu::GH_posi = -1;
int boardMenu::Ghsz = 1;


boardMenu::boardMenu(int code, int w, int h, int max, int _null)
{
	width =		w;
	height =	h;
	cMAX =		max;
	cNull =		_null;
	kI =		new input_key(code);

	if (GH_posi == -1)
	{
		GH_posi = LoadGraph("textures/icon/posi.png");
	}
}
boardMenu::~boardMenu()
{
	delete kI;
}

int boardMenu::Update()
{
	kI->Update();

	vector2D<int> sign = getDirectionSigns(kI->getDirectionEdge());
	cp.x += sign.x;
	cp.y += sign.y;

	if (sign.x != 0 || sign.y != 0)
	{
		soundMgr::GetInstance().Play(SE_Select);
	}
	cp = normalization(cp);
	cID = createID(cp);
	cID_last = -1;

	if (cID >= 0)
	{	
		if (kI->getButtonEdge(actButton_A))
		{
			cID_last = (cID < cMAX) ? cID : cNull;
			soundMgr::GetInstance().Play(SE_Decide);
			return 1;
		}
		if (kI->getButtonEdge(actButton_B))
		{
			soundMgr::GetInstance().Play(SE_BackSpace);
			return -1;
		}
	}
	return 0;
}
void boardMenu::Draw(int bx, int by)
{
	int cx1 = 0;
	int cy1 = 0;
	int cx2 = 0;
	int cy2 = 0;

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int dx1 = bx + x * GH_panels[0].width;
			int dy1 = by + y * GH_panels[0].height;
			int dx2 = dx1 + GH_panels[0].width;
			int dy2 = dy1 + GH_panels[0].height;

			DrawBox(dx1, dy1, dx2, dy2, GetColor(128, 128, 128), TRUE);

			int tmpID = createID({ x, y });

			if (tmpID < Ghsz)
			{
				dx1 = bx + x * GH_panels[tmpID].width;
				dy1 = by + y * GH_panels[tmpID].height;
				dx2 = dx1 + GH_panels[tmpID].width;
				dy2 = dy1 + GH_panels[tmpID].height;
				DrawExtendGraph(dx1, dy1, dx2, dy2, GH_panels[tmpID].handle, TRUE);
			}
			if (cID == tmpID)
			{
				cx1 = dx1 - GH_panels[0].width * 0.2;
				cy1 = dy1 - GH_panels[0].height * 0.2;
				cx2 = dx2 + GH_panels[0].width * 0.2;
				cy2 = dy2 + GH_panels[0].height * 0.2;
			}
		}
	}
	if (cID >= 0 && cID < cMAX)
	{
		DrawExtendGraph(cx1, cy1, cx2, cy2, GH_panels[cID].handle, TRUE);
	}
	DrawGraph(cx2 - 20, cy2 - 20, GH_posi, TRUE);

	if (!labelStr.empty())
	{
		DrawBox(bx - 6, by - 32, bx + 100, by, GetColor(64, 64, 64), TRUE);
		DrawFormatString(bx, by - 24, GetColor(255, 255, 255), labelStr.c_str());
	}
}
void boardMenu::setCusorID(int cID)
{
	this->cID = cID; 
	cp = createCp(cID);
}

void boardMenu::setGraphics(graphicParam* gh, int sz) 
{
	GH_panels = gh; 
	Ghsz = sz;
}
vector2D<int> boardMenu::createCp(int cID)
{
	vector2D<int> retval;
	retval.y = (cID == 0) ? 0 : cID / width;
	retval.x = cID - (width * retval.y);
	return retval;
}
vector2D<int> boardMenu::normalization(vector2D<int> _cp)
{
	if (_cp.x < 0)
	{
		_cp.x = width - 1;
	}
	if (_cp.x >= width)
	{
		_cp.x = 0;
	}
	if (_cp.y < 0)
	{
		_cp.y = height - 1;
	}
	if (_cp.y >= height)
	{
		_cp.y = 0;
	}
	return _cp;
}