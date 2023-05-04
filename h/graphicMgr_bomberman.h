#pragma once
#include "../h/global.h"
#include "../h/userProfile.h"

enum
{
	pose_walk,
	pose_winner,
	pose_loser,
	pose_hold,
	pose_rocketCharge,
	pose_rocketJump,
	pose_badBomber,

	pose_MAX
};

class graphicMgr_bomberman
{
public:
	graphicMgr_bomberman					();
	~graphicMgr_bomberman					();

	void					Draw			(int x, int y, double exrate = 1.0, int _dir = Direction_Down, int n = 0, bool reverse = false);
	void					DrawIcon		(int x, int y, int d = Direction_Down);
		
	void					setPlayerID		(int id)				{ playerID = id; }
	void					setTeamID		(int id)				{ teamID = id; }
	void					setCostumeID	(Costume parts)			{ costumeIDs = parts; }
	void					setFaceID		(int id)				{ faceID = id; }
	void					setState		(int _state)			{ state = _state; }

	int						getFaceID		()						{ return faceID; }

private:
	struct bombermanSet
	{
		int					walk[Direction_MAX][plGraph_MAX] = { -1 };
		int					hold[Direction_MAX][plGraph_MAX] = { -1 };
		int					win[plGraph_MAX] = { -1 };
		int					lose[2] = { -1 };
		int					ride[Direction_MAX] = { -1 };
		int					rocket_charge[2] = { -1 };
		int					rocket_jump[plGraph_MAX] = { -1 };
	};
	static bombermanSet		GH_bomberman[PLAYER_MAX];
	static bombermanSet**	GH_costume;
	static bombermanSet		GH_frame;
	static bool				firstTime;

	Costume					costumeIDs;
	int						playerID = 0;
	int						teamID = Team_None;
	int						faceID = 0;
	int						state = 0;
	static int				GH_faces[5][Direction_MAX][plGraph_MAX];
	static int				GH_badBomber[Direction_MAX];
	static int				GH_rocket_charge[2];
	static int				GH_rocket_jump[plGraph_MAX];

	void					DrawFrame		(int x, int y, double exrate, int _dir, int n, bool reverse);
	static void				LoadCostume		();
};