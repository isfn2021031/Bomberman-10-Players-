#pragma once
#include <vector>
#include <string>
#include "../h/Singleton.h"

enum
{
	Debug_None,
	Debug_User,
	Debug_Player,
	Debug_Global,
	Debug_Items,
	Debug_Elements,
	Debug_Cheat,
	Debug_FullPower,
	Debug_RandomBombs,

	Debug_MAX
};

using namespace std;

class DebugMgr :public Singleton<DebugMgr>
{
public:
	friend class Singleton<DebugMgr>;	// Singletonでのインスタンス生成は許可
	void			Update		();
	void			Draw		();
	
	void			pushStr		(string str)	{ dispStr.push_back(str); }
	void			setFPS		(double fps)	{ FPS = fps; }
	int				getMode		()				{ return mode; }
	int				getTargetID	()				{ return targetID; }
	
private:
	vector<string>	dispStr;
	int				mode = Debug_None;
	int				targetID = 0;
	int				OldKey_shiftR = 0;
	int				OldKey_shiftL = 0;
	int				OldKey_ctrlR = 0;
	int				OldKey_ctrlL = 0;
	double			FPS = 60;

					DebugMgr	();
	virtual			~DebugMgr	();
	void			keyInput	();
};