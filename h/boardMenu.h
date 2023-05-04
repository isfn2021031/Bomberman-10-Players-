#pragma once
#include <vector>
#include "../h/input_key.h"
#include "../h/graphicParam.h"

class boardMenu
{
public:
	boardMenu						(int code, int w, int h, int max, int _null = -1);
	virtual			~boardMenu		();
	
	int				Update			();
	void			Draw			(int bx, int by);
		
	void			setGraphics		(graphicParam* gh, int sz);
	void			setCusorID		(int cID);
	void			setLabelStr		(string str)			{ labelStr = str; }

	input_key*		getInputKey		()						{ return kI; }
	int				getlastID		()						{ return cID_last; }

private:
	input_key*		kI = nullptr;

	vector2D<int>	cp = { 0,0 };
	graphicParam*	GH_panels;
	string			labelStr = "";

	int				width = 2;
	int				height = 2;
	int				cID = 0;
	int				cID_last = -1;
	int				cNull = -1;
	int				cMAX = 4;
	static int		GH_posi;
	static int		Ghsz;

	vector2D<int>	normalization	(vector2D<int> _cp);
	vector2D<int>	createCp		(int cID);
	int				createID		(vector2D<int> _cp)		{ return _cp.x + _cp.y * width; }
};