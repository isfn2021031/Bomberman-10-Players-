#pragma once
#include <vector>
#include <string>
#include "../h/Singleton.h"
#include "../h/effect_rotaGraph3.h"
#include "../h/graphicParam.h"

#define	N_MAX					(10)

using namespace std;

class graphicMgr_common:public Singleton<graphicMgr_common>
{
public:
	graphicMgr_common							();
	~graphicMgr_common							();

	void					DrawBox				(int x1, int y1, int x2, int y2);
	void					DrawFlag			(int id, int x, int y);
	void					DrawPlayerName		(int id, int x, int y, bool npc = false);
	void					DrawControllerIcon	(int type, int x, int y);
	void					DrawItemIcon		(int type, int x1, int y1, int x2 = -1, int y2 = -1);
	void					DrawArrow			(int x,int y,int d);
	void					DrawStar			(int x, int y);
	void					DrawValue			(int& x, int y, double ExRate, int value, const int	WIDTH = 2);

	graphicParam			getBuffHandle		(int bufftype)									{ return GH_buffs[bufftype]; }
	graphicParam*			getItemHandles		()												{ return GH_items; }
	graphicParam*			getFlagsptr			()												{ return GH_flags; }

private:
	effect_rotaGraph3**		eff_plNames = nullptr;
	
	graphicParam			GH_box;
	graphicParam			GH_flag;
	graphicParam			GH_arrow;
	graphicParam			GH_star;
	graphicParam			GH_plNameCOM;
	graphicParam			GH_numbers[N_MAX];
	graphicParam*			GH_plNames = nullptr;
	graphicParam*			GH_flags = nullptr;
	graphicParam*			GH_items = nullptr; 
	graphicParam*			GH_controllerIcons = nullptr;
	graphicParam*			GH_arrows = nullptr;
	graphicParam*			GH_buffs = nullptr;
};