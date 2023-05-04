#pragma once
#include "../h/configMgr.h"
#include "../h/network_common.h"

using namespace std;

class configMgr_number :public configMgr
{
public:
	configMgr_number				(int _max);
	int					Update		()			override;
	void				Draw		()			override;

protected:
	virtual bool		mallocMI	(int _step)	override;
	
private:
	int					IP_address[IPV4_SEPARATE] = { 127,0,0,1 };
};