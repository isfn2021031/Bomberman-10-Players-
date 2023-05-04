#include <string>
#include "DxLib.h"
#include "../h/buff_base.h"
#include "../h/graphicMgr_common.h"
#include "../h/playerCounter.h"
#include "../h/RGB.h"
#include "../h/itemPanel.h"

graphicMgr_common::graphicMgr_common()
{	
	GH_box =		LoadGraphParam("textures/Box.png");
	GH_flag =		LoadGraphParam("textures/icon/flag.png");
	GH_star =		LoadGraphParam("textures/icon/star.png");
	GH_plNameCOM =	LoadGraphParam("textures/icon/pl_name_com.png");
	GH_arrow =		LoadGraphParam("textures/icon/arrow.png");

	GH_items =				new graphicParam[itemPanel_MAX];
	GH_controllerIcons =	new graphicParam[ctr_MAX];
	GH_plNames =			new graphicParam[PLAYER_MAX];
	GH_arrows =				new graphicParam[Direction_MAX];
	GH_buffs =				new graphicParam[BuffType_MAX];
	GH_flags =				new graphicParam[Team_MAX];

	LoadDivGraphParam("textures/Numbers.png",			N_MAX,			N_MAX,			1,			18,			36,			GH_numbers);
	LoadDivGraphParam("textures/icon/itempanels.png",	itemPanel_MAX,	7,				6,			CELL_SIZE,	CELL_SIZE,	GH_items);
	LoadDivGraphParam("textures/icon/ctr.png",			ctr_MAX,		ctr_MAX,		1,			64,			64,			GH_controllerIcons);
	LoadDivGraphParam("textures/icon/pl_names.png",		PLAYER_MAX,		1,				PLAYER_MAX, 34,			30,			GH_plNames);
	LoadDivGraphParam("textures/icon/arrows.png",		Direction_MAX,	Direction_MAX,	1,			CELL_SIZE,	CELL_SIZE,	GH_arrows);
	LoadDivGraphParam("textures/icon/buff.png",			BuffType_MAX,	BuffType_MAX,	1,			22,			22,			GH_buffs);

	int tmp_screen = GetDrawScreen();
	RgbData tmp_bright;
	GetDrawBright(&tmp_bright.r, &tmp_bright.g, &tmp_bright.b);

	for (int i = 0; i < Team_MAX; i++)
	{
		GH_flags[i].width = GH_flag.width;
		GH_flags[i].height = GH_flag.height;
		GH_flags[i].handle = MakeScreen(GH_flag.width, GH_flag.height, TRUE);
		
		if (i != Team_None)
		{
			while (CheckHandleASyncLoad(GH_flags[i].handle) && ProcessMessage() == 0) {}
			SetDrawScreen(GH_flags[i].handle);
			ClearDrawScreen();
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			SetDrawBrightTeam(i);
			DrawGraph(0, 0, GH_flag.handle, TRUE);
			SetDrawBright(tmp_bright.r, tmp_bright.g, tmp_bright.b);
			SetDrawScreen(tmp_screen);
		}
	}
	
	eff_plNames = new effect_rotaGraph3* [PLAYER_MAX];
	
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		eff_plNames[i] = new effect_rotaGraph3(GH_arrow, 0.05);
	}
}
graphicMgr_common::~graphicMgr_common()
{
	delete[] GH_plNames;

	for (int i = 0; i < Team_MAX; i++)
	{
		DeleteGraph(GH_flags[i].handle);
	}
	delete[] GH_flags;
	delete[] GH_controllerIcons;
	delete[] GH_arrows;
	delete[] GH_buffs;

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		delete eff_plNames[i];
	}
	delete[] eff_plNames;
}

void graphicMgr_common::DrawFlag(int id, int x, int y)
{
	if (id >= 0 && id < Team_None)
	{
		DrawGraph(x, y, GH_flags[id].handle, TRUE);
	}
}
void graphicMgr_common::DrawBox(int x1, int y1, int x2, int y2)
{
	DrawExtendGraph(x1, y1, x2, y2, GH_box.handle, TRUE);
}
void graphicMgr_common::DrawPlayerName(int id, int x, int y, bool npc)
{
	int gh = !npc ? GH_plNames[id].handle : GH_plNameCOM.handle;

	if (!npc)
	{
		if (eff_plNames[id] != nullptr)
		{
			eff_plNames[id]->Update();

			int eff_y = y + GH_plNames[id].height * 0.8;

			SetDrawBright(255, 255, 255);
			eff_plNames[id]->Draw(x - 1, eff_y - 1);

			SetDrawBright(128, 128, 128);
			eff_plNames[id]->Draw(x + 1, eff_y + 1);

			SetDrawBrightPlayer(id);
			eff_plNames[id]->Draw(x, y + GH_plNames[id].height * 0.8);
		}
	}
	SetDrawBright(255, 255, 255);
	DrawRotaGraph(x - 2, y - 1, 1.0, 0.0, gh, TRUE);

	SetDrawBright(128, 128, 128);
	DrawRotaGraph(x + 2, y + 2, 1.0, 0.0, gh, TRUE);

	SetDrawBrightPlayer(id);
	DrawRotaGraph(x, y, 1.0, 0.0, gh, TRUE);

	SetDrawBright(255, 255, 255);
}
void graphicMgr_common::DrawControllerIcon(int type, int x, int y)
{
	if (type < ctr_MAX)
	{
		DrawGraph(x, y, GH_controllerIcons[type].handle, TRUE);
	}
}
void graphicMgr_common::DrawItemIcon(int type, int x1, int y1, int x2, int y2)
{
	if (type < itemPanel_MAX)
	{
		if (x2 == -1)
		{
			x2 = x1 + GH_items[type].width;
		}
		if (y2 == -1)
		{
			y2 = y1 + GH_items[type].height;
		}
		DrawExtendGraph(x1, y1, x2, y2, GH_items[type].handle, TRUE);
	}
}
void graphicMgr_common::DrawArrow(int x, int y, int d)
{
	if (d < Direction_MAX)
	{
		DrawGraph(x, y, GH_arrows[d].handle, TRUE);
	}
}
void graphicMgr_common::DrawStar(int x, int y)
{
	DrawGraph(x, y, GH_star.handle, TRUE);
}
void graphicMgr_common::DrawValue(int& x, int y, double ExRate, int value, const int WIDTH)
{
	// •ª•ê
	int	n = 1;

	for (int i = 0; i < WIDTH; i++)
	{
		int q = value % (n * N_MAX);
		int idx = q / n;
		n *= N_MAX;
		DrawRotaGraph(x, y, ExRate, 0.0, GH_numbers[idx].handle, TRUE);
		x -= GH_numbers[idx].width * ExRate;
	}
}