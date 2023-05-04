#pragma once
#include "../h/configMgr.h"

class configMgr_other :public configMgr
{
public:
	configMgr_other	();
	~configMgr_other()override;
	
private:
	static const menuData	mDatalist[sCONFIG_MAX];
};