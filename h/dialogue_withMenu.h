#pragma once
#include <string>
#include "../h/dialogue.h"
#include "../h/menuCursorMgr.h"

using namespace std;

class dialogue_withMenu :public dialogue
{
public:
	dialogue_withMenu							(string str);
	virtual					~dialogue_withMenu	();
	virtual int				Update				()override;
	virtual void			Draw				()override;

protected:
	menuCursorMgr*			mI_ptr = nullptr;

private:
	int						cur = 0;
};