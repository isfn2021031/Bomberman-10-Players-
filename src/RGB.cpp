#include "DxLib.h"

#include "../h/RGB.h"
#include "../h/global.h"

const RgbData PlayerColors[PLAYER_MAX] =
{
	{WHITE,		WHITE,		WHITE,		RGB_WHITE} ,
	{GRAY,		GRAY,		GRAY,		RGB_GRAY},
	{RED_R,		RED_G,		RED_B,		RGB_RED},
	{BLUE_R,	BLUE_G,		BLUE_B,		RGB_BLUE},
	{GREEN_R,	GREEN_G,	GREEN_B,	RGB_GREEN},
	{YELLOW_R,	YELLOW_G,	YELLOW_B,	RGB_YELLOW},
	{MAGENTA_R,	MAGENTA_G,	MAGENTA_B,	RGB_MAGENTA},
	{CYAN_R,	CYAN_G,		CYAN_B,		RGB_CYAN},
	{ORANGE_R,	ORANGE_G,	ORANGE_B,	RGB_ORANGE},
	{PURPLE_R,	PURPLE_G,	PURPLE_B,	RGB_PURPLE},
};
const RgbData TeamColors[Team_MAX] =
{
	{RED_R,		RED_G,		RED_B,		RGB_RED},
	{BLUE_R,	BLUE_G,		BLUE_B,		RGB_BLUE},
	{GREEN_R,	GREEN_G,	GREEN_B,	RGB_GREEN},
	{YELLOW_R,	YELLOW_G,	YELLOW_B,	RGB_YELLOW},
	{MAGENTA_R,	MAGENTA_G,	MAGENTA_B,	RGB_MAGENTA},
	{CYAN_R,	CYAN_G,		CYAN_B,		RGB_CYAN},
	{ORANGE_R,	ORANGE_G,	ORANGE_B,	RGB_ORANGE},
	{PURPLE_R,	PURPLE_G,	PURPLE_B,	RGB_PURPLE},

	{WHITE,		WHITE,		WHITE,		RGB_WHITE} ,
};

bool SetDrawBrightPlayer(int id)
{
	if (id < 0 || id >= PLAYER_MAX)
	{
		SetDrawBright(255, 255, 255);
		return false;
	}
	SetDrawBright(PlayerColors[id].r, PlayerColors[id].g, PlayerColors[id].b);
	return true;
}
bool SetDrawBrightTeam(int id)
{
	if (id < 0 || id >= Team_None)
	{
		SetDrawBright(255, 255, 255);
		return false;
	}
	SetDrawBright(TeamColors[id].r, TeamColors[id].g, TeamColors[id].b);
	return true;
}