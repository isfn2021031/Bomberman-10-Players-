#include <string>
#include <fstream>
#include "DxLib.h"
#include "../h/handicap.h"
#include "../h/graphicMgr_common.h"

handicap::handicap(int id)
{
	playerID = id;
	graphMgr.setPlayerID(id);
	load();
}
handicap::~handicap()
{
	fstream fout(FileName, ios::binary | ios::out);

	if (!fout)
	{
		return;
	}
	for (int i = 0; i < HANDI_SIZE; i++)
	{
		fout << items[i] << " ";
	}
	fout.close();
}

void handicap::Draw(int x, int y)
{
	graphMgr.DrawIcon(x, y);

	int x2 = x;
	for (int i = 0; i < HANDI_SIZE; i++)
	{
		x2 += CELL_SIZE;

		if (i == idx)
		{
			cp.x = x2;
			cp.y = y;
		}
		DrawBox(x2, y, x2 + CELL_SIZE, y + CELL_SIZE, GetColor(255, 255, 255), TRUE);
		DrawBox(x2 + 1, y + 1, x2 + CELL_SIZE - 1, y + CELL_SIZE - 1, GetColor(0, 0, 0), TRUE);

		if (items[i] != itemPanel_None)
		{
			graphicMgr_common::GetInstance().DrawItemIcon(items[i], x2, y);
		}
	}
}
void handicap::DrawCursor()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawBox(cp.x - 6, cp.y - 6, cp.x + CELL_SIZE + 6, cp.y + CELL_SIZE + 6, GetColor(200, 100, 10), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

bool handicap::load()
{
	FileName = "bin/handi/" + to_string(playerID) + ".bin";

	fstream fin(FileName, ios::binary | ios::in);

	if (!fin)
	{
		for (int i = 0; i < HANDI_SIZE; i++)
		{
			items[i] = itemPanel_None;
		}
		return false;
	}
	for (int i = 0; i < HANDI_SIZE; i++)
	{
		fin >> items[i];
	}
	fin.close();
	return true;
}
bool handicap::add(int itemType)
{
	items[idx++] = itemType;

	if (idx >= HANDI_SIZE)
	{
		return true;
	}
	return false;
}
bool handicap::sub()
{
	return (--idx < 0);
}
