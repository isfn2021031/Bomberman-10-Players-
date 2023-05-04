#pragma once
#include <vector>
#include <string>
#include "../h/input_key.h"

class menu_cursor
{
public:
	int n = 0;
	int min = 0;
	int max = 1;

	menu_cursor operator +(const menu_cursor& r)
	{
		menu_cursor rv;
		rv.n = this->n + r.n;
		if (rv.n >= this->max)
		{
			rv.n = this->min + (rv.n - this->max);
		}
		else if (rv.n < this->min)
		{
			rv.n = this->max + (rv.n + this->min);
		}
		return rv;
	}
};
class menuCursorMgr
{
public:
	menuCursorMgr								(int max = 1, bool movLast = false);
	virtual					~menuCursorMgr		();

	virtual int				Update				();
	virtual void			Draw				(int px, int py);

	void					DrawLabelStr		(int x1, int y1, int x2, int y2);
	virtual void			DrawGuideStr		(int x1, int y1, int x2, int y2);
	void					DrawGuideStrSingle	(int x1, int y1, int x2, int y2);

	void					normalization		();

	//////////////////////////////////////////////////	setter	///////////////////////////////////////////////////////////////	
	void					setKeyCode			(int code);
	void					setCursor			(int c)						{ cursor = c; }
	void					setRange			(vector2D<pixelPosi> range) { BoxRange = range; }
	virtual void			setCursorStr		(vector<string> strings)	{ cursorStr = strings; }
	void					setGuideStr			(vector<string> strings)	{ guideStr = strings; }
	void					setLock				(vector<bool> locks)		{ lock = locks; }
	void					setLabelStr			(string str)				{ labelStr = str; }
	void					setGuideStrSingle	(string str)				{ guideStrSingle = str; }
	void					setDecideButtonID	(int id);
	void					setBackButtonID		(int id);
	void					setAccentGraph		(int c, string FileName);
	virtual void			setRestInput		(bool f)					{ restInput = f; }

	//////////////////////////////////////////////////	getter	///////////////////////////////////////////////////////////////	
	input_key*				getInputKey			()							{ return kI; }
	vector2D<pixelPosi>		getRange			()							{ return BoxRange; }
	string					getNowCursorStr		(); 
	int						getCursorMAX		()							{ return cursorMAX; }
	int						getCursor			()							{ return cursor; }
	bool					getPause			()							{ return pause; }

	virtual int				getSign				();
	virtual int				output				();

protected:
	input_key*				kI = nullptr;
	vector<string>			cursorStr;
	vector<string>			guideStr;
	vector<int>				GH_accents;
	vector<bool>			lock;
	string					labelStr;
	string					guideStrSingle;

	bool					dynamic_kI = true;
	bool					restInput = false;
	bool					pause = false;
	int						cursorMAX = 1;
	int						cursor = 0;
	
	virtual void			createCursor		();
	
protected:
	vector2D<pixelPosi>		BoxRange = { 256,32 };
	static int				GH_window;
	static int				GH_mbox;
	static int				GH_mCursor;

private:
	bool					movLast = false;
};