#pragma once
#include "../h/textAnimation.h"

using namespace std;

class dialogue
{
public:
	dialogue						(string str);
	virtual			~dialogue		();
	virtual int		Update			();
	virtual void	Draw			();

protected:
	textAnimation*	txAnime_ptr = nullptr;
	int				finishAnimationMAX = 20;
	int				finishAnimationCnt = 0;

private:
	static int		GH_win;
};