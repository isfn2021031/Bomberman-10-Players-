#pragma once
#include "../h/global.h"
#include "../h/bomb.h"
#include "../h/status_param.h"
#include "../h/itemPanel.h"
#include "../h/buff_base.h"
#include "../h/graphicMgr_bomberman.h"

class status 
{
public:
	status										(int id, int tID);
	~status										();

	void					Update				();
	void					Draw				();
	void					DrawName			(int x, int y);
	
	void					pl_down				();
	void					addScore			(int n, bool se);
	void					addHitPoint			(int n, bool se);
	void					addFirePower		(int n, bool se);
	void					addSpeed			(int n, bool se);
	void					addBombCapacity		(int n, bool se);

	static int				convertItemToBomb	(int i);

	//////////////////////////////////////////////////	getter	///////////////////////////////////////////////////////////////	
	statusParamMgr*			getStatusParam		()								{ return parameters; }

	bool					getCanKick			()								{ return parameters->kick_bombPass == 1; }
	bool					getLineBomb			()								{ return parameters->powerGlove_lineBomb == 3; }
	bool					getBombPass			()								{ return parameters->kick_bombPass == 2; }
	bool					getBombChange		()								{ return parameters->shield_change_rocket == 3; }
	bool					getRocket			()								{ return parameters->shield_change_rocket == 4; }	
	
	int						getThrowPower		();
	int						getScore			()								{ return score->getValue(); }
	int						getFirePower		();
	int						getBombCapacity		();
	int						getBombType			();
	int						getShield			()								{ return parameters->shield_change_rocket; }

	pixelPosi				getSpeed			();
	buff_base*				getBuff				(int bufftype)					{ return pBuffs[bufftype]; }
	vector<int>				getItemHistory		()								{ return itemHistory; }

	//////////////////////////////////////////////////	setter	///////////////////////////////////////////////////////////////	
	int						set					(int type, bool se = true);
	static void				setStyle			(int _style)					{ style = _style; }
	void					setBombType			(int n, bool se);
	void					setBombKick			(int n, bool se);
	void					setBombThrow		(int n, bool se);
	void					setShield			(int n, bool se);
	void					setBuff				(int bufftype, int pattern = -1, int fc = 0, bool existence = true);	
	void					setDefault			(vector<int> vec = {});

private:
	enum
	{
		statusType_Heart,
		statusType_Fire,
		statusType_Speed,
		statusType_BombCapacity,
		statusType_BombKick,
		statusType_BombPunch,
		statusType_PowerGlove5,
		statusType_PowerGlove3,
		statusType_Shield,
		statusType_AutomaticShield,
		statusType_LineBomb,
		statusType_BombPass,
		statusType_SoftBlockPass,
		statusType_BombChange,
		statusType_Rocket,

		statusType_MAX
	};
	vector<int>				itemHistory;
	parameter*				score = nullptr;
	statusParamMgr*			parameters = nullptr;
	buff_base*				pBuffs[BuffType_MAX] = { nullptr };
	graphicMgr_bomberman	graphMgr;
	
	static bool				firstTime;
	static int				GH_playerIcons[PLAYER_MAX];
	static int				GH_status[statusType_MAX];
	static int				GH_bombIcon[BombType_MAX]; 
	static int				style;
	int						playerID = 0;
	int						teamID = 0;

	void					setAbility				(bool& ptr, bool f, bool se);
};