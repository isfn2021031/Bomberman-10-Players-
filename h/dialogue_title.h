#pragma once
#include "../h/dialogue.h"

using namespace std;

class dialogue_title :public dialogue
{
public:
	dialogue_title							();
	virtual					~dialogue_title	();
	virtual void			Draw			();

private:
	static int				GH_titleArt;
};