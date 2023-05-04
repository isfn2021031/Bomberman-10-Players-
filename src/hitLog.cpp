#include "DxLib.h"
#include "../h/hitLog.h"
#include "../h/RGB.h"

int hitLog::GH_Hit = -1;

hitLog::hitLog()
{
	if (GH_Hit == -1)
	{
		GH_Hit = LoadGraph("textures/Hit.png");
	}
	messages.clear();
}
hitLog::~hitLog()
{
	messages.clear();
}

void hitLog::Update()
{
	for (int i = 0; i < messages.size(); i++)
	{
		messages[i].frameCnt++;
	}
	if (!messages.empty())
	{
		if ((messages[0].frameCnt) > 200)
		{
			messages.pop_front();
		}
	}
}
void hitLog::Draw()
{
	RgbData rgb;
	GetDrawBright(&rgb.r, &rgb.g, &rgb.b);
	SetDrawBrightPlayer(playerID);
	
	int i = 0;
	for (auto msg : messages)
	{
		DrawGraph(*msg.x, *msg.y - 8 * i, GH_Hit, TRUE);
		i++;
	}
	SetDrawBright(rgb.r, rgb.g, rgb.b);
}

void hitLog::Addition(int* x, int* y)
{
	message msg;
	msg.x = x;
	msg.y = y;
	messages.push_back(msg);
}