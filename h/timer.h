#pragma once
#include "DxLib.h"
#include "../h/graphicParam.h"

class timer
{
public:
							timer			();
	void					Update			();
	void					Draw			();

	bool					getStart		(int sec);
	int						getFrameCnt		() { return FrameCnt; }
	int						getRemainingTime() { return remainingTime; }

private:
	struct minute
	{
		int					min = 0;
		int					sec = 0;
	};
	// Œo‰ßŽžŠÔ
	int						FrameCnt = 0;
	int						configTime = 0;
	int						remainingTime = 0;

	static graphicParam		GH_timer;
	static graphicParam		GH_colon;
	
	minute					convertSecondToMinute(int sec);
	void					drawMinute		(int x, int y, double ExRate, minute value);
};