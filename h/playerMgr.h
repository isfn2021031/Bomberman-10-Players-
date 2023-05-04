#pragma once
#include "../h/elementMgr.h"
#include "../h/player.h"

class playerMgr :public elementMgr
{
public:
	playerMgr											();
	virtual							~playerMgr			()override;
	void							InitializeBySudden	();
	void							Update				()override;
	void							DrawBottomLayer		();
	void							DrawTopLayer		();
	
	void							setNowGame			(bool f);
	vector<vector2D<int>>			Positioning			(int pattern);
	bool							CheckGameOver		(bool timeUp = false);

private:
	player*							elements_pl[ELEMENT_MAX] = { nullptr };
};