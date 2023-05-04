#pragma once
#include <vector>
#include <map>
#include "../h/Singleton.h"
#include "../h/global.h"
#include "../h/graphicMgr_bomberman.h"
#include "../h/boardMenu.h"
#include "../h/configMgr.h"
#include "../h/userProfile.h"

using namespace std;

enum
{
	ctr_Empty,
	ctr_COM,
	ctr_Keyboard,
	ctr_Mouse,
	ctr_Gamepad,
	ctr_KeyPad,
	ctr_Network,

	ctr_MAX,
};

class playerCounter :public Singleton<playerCounter>
{
public:
	friend class				Singleton<playerCounter>;
	void						Initialize		();
	bool						Update			();
	void						Draw			();
	void						ReSetup			();

	userProfile					getProfile		(int pID)							{ return profiles[pID]; }
	string						getNameString	(int id);	
	int							getActiveID		(int _idx);
	int							getTeamID		(int pID)							{ return teamIDs[pID]; }
	int							getTotal		();
	int							getPlayerNum	(int type);
	int							getCtr			(int id)							{ return plCtr[id]; }
	bool						getNpc			(int pID)							{ return isNPC[pID]; }
	
	void						setNameString	(int pId, string str);
	void						setPlayerCtr	(int id, int type);
	
	void						setTeamID		(int pID, int tID)					{ teamIDs[pID] = tID; }
	void						setProfile		(int pID, userProfile prof);
	void						setNpc			(int pID, bool f)					{ isNPC[pID] = f; }

private:
	playerCounter								();
	virtual						~playerCounter	();

	void						mallocWaitMenu	(int pId, int m = 0);

	input_key*					kI_ptr		[PLAYER_MAX] = { nullptr };
	menuCursorMgr*				mI_ptr		[PLAYER_MAX] = { nullptr };
	boardMenu*					teamColor	[PLAYER_MAX] = { nullptr };
	configMgr*					cf_ptr		[PLAYER_MAX] = { nullptr };

	userProfile					profiles	[PLAYER_MAX];
	string						plNames		[PLAYER_MAX];
	string						padNames	[PLAYER_MAX];
	graphicMgr_bomberman		graphMgr	[PLAYER_MAX];
	map<string, userProfile>	mp_profiles;

	int							frameCnt = 0;
	int							JoyPadNum = 1;

	int							plCtr		[PLAYER_MAX] = { ctr_Empty };
	int							teamIDs		[PLAYER_MAX] = { Team_None };
	int							nowMode		[PLAYER_MAX] = { 0 };

	bool						strPattern = false;
	bool						isNPC		[PLAYER_MAX] = { false };
	bool						nowUpdate = false;
};