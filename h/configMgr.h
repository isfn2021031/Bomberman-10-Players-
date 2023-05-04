#pragma once
#include <string>
#include <vector>
#include "../h/configLoader.h"
#include "../h/menuCursorMgr_XdispSingle.h"
#include "../h/dialogue.h"
#include "../h/language.h"

using namespace std;

struct menuData
{
	int					Max = 0;
	int					cursor = 0;
	langData			labelStr = { "" ,"" };
	vector<langData>	cursorStr;
	langData			guideStrSingle = { "" ,"" };
};

class configMgr
{
public:
	configMgr						(int);
	virtual				~configMgr	();
	virtual int			Update		();
	virtual void		Draw		(); 
	virtual void		Draw		(int px, int py) {}

	virtual void		Finalize	() {}

	int					getStep		() { return step; }
	menuCursorMgr*		getMI		() { return mI; }

	//////////////////////////////////////////////////	setter	///////////////////////////////////////////////////////////////	
	void				setStep		(int _step);
	void				setData		(int _step, menuData md)			{ mVec[_step] = md; }
	void				setSave		(bool flag, string _fileName = "");
	void				setDialogue	(bool flag)							{ useDialogue = flag; }

	static string		getPadNameFromPadId(int padID);
	static int			getPadIdFromPadName(string padName);

protected:
	vector<menuData>	mVec;
	menuCursorMgr*		mI = nullptr;
	string				saveFileName = "bin/out.bin";

	bool				autoSave = false;
	bool*				unused = nullptr;

	int					BasisDX = (int)(WINDOW_WIDTH * 0.3);
	int					BasisDY = (int)(WINDOW_HEIGHT * 0.20);
	int					Interval = 30;
	int					stepMAX = 1;
	int					step = 0;
	static int          GH_marrow[2];

	virtual bool		mallocMI			(int _step);
	void				load				(menuData md);
	void				DrawDialogue		();

private:
	dialogue*			dialogue_ptr = nullptr;
	bool				save = true;
	bool				useDialogue = true;
};