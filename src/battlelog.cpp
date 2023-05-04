#include "DxLib.h"
#include "../h/battlelog.h"
#include "../h/global.h"
#include "../h/graphicMgr_common.h"
#include "../h/RGB.h"
#include "../h/playerCounter.h"
#include "../h/soundMgr.h"

#define SEPARATE	(WINDOW_WIDTH / PLAYER_MAX)

int battlelog::GH_crown = -1;
int	battlelog::GH_battlelog[battlelog_MAX] = { -1 };

battlelog::battlelog(int id, int tId, bool _npc)
{
	playerID = id;
	teamID = tId;
	npc = _npc;

	bomberGH.setPlayerID(id);
	bomberGH.setTeamID(tId);

	if (GH_crown == -1)
	{
		GH_crown = LoadGraph("textures/icon/crown.png");
	}
	if (GH_battlelog[0] == -1)
	{
		LoadDivGraph("textures/icon/battlelog.png", battlelog_MAX, 1, battlelog_MAX, 32, 32, GH_battlelog);
	}
	real_labels = language::getInstance()->getStringVec(Labels);
}
battlelog::~battlelog()
{
	if (active)
	{
		userProfile prof = playerCounter::GetInstance().getProfile(playerID);

		if (winner)
		{
			prof.total_wins++;
		}

		prof.total_matches++;
		prof.total_score += counters[battlelog_Score];
		prof.total_hits += counters[battlelog_Hits];
		prof.total_damages += counters[battlelog_Falls];
		prof.total_sds += counters[battlelog_SDs];
		prof.total_bombs += counters[battlelog_Bombs];

		playerCounter::GetInstance().setProfile(playerID, prof);
	}
}

bool battlelog::Update()
{
	if (!active)
	{
		active = true;

		SeCache cache;
		cache.frameCnt = 12 + ranking * 8;
		cache.seIdx = SE_BackSpace;
		soundMgr::GetInstance().CreateCache(cache);
	}
	if (basisDY)
	{
		basisDY -= accel;
		
		if (basisDY < 64)
		{
			if (accel > 1)
			{
				accel--;
			}
			else
			{
				accel = 1;
			}
		}
	}
	else
	{
		basisDY = 0;

		if (winner)
		{
			if (starWaitCnt)
			{
				starWaitCnt--;
			}
			else
			{
				if (starExRate > 1.0)
				{
					if (starExRate == 50.0)
					{
						bomberGH.setFaceID(3);
						soundMgr::GetInstance().Play(SE_LifeUp);
					}
					starExRate -= 2.5;
				}
				else
				{
					starExRate = 1.0;
				}
			}
		}
		return true;
	}
	return false;
}
void battlelog::Draw()
{
	int h = 32;
	int dx = playerID * SEPARATE;
	int dy = basisDY + (ranking * h);

	RgbData color;
	GetDrawBright(&color.r, &color.g, &color.b);
	SetDrawBrightTeam(teamID);
	graphicMgr_common::GetInstance().DrawBox(dx, dy, dx + SEPARATE, dy + h);
	SetDrawBright(color.r, color.g, color.b);

	bomberGH.DrawIcon(dx, dy);
	graphicMgr_common::GetInstance().DrawFlag(teamID, dx, dy);

	for (int l = 0; l < battlelog_MAX; l++)
	{
		int dy2 = dy + h * (l + 1);

		SetDrawBright(255, 255, 255);
		graphicMgr_common::GetInstance().DrawBox(dx, dy2, dx + SEPARATE, dy2 + h);

		if (!npc)
		{
			SetDrawBright(224, 96, 10);
			graphicMgr_common::GetInstance().DrawBox(dx, dy2, dx + SEPARATE, dy2 + h);
			SetDrawBright(255, 255, 255);
		}
		DrawGraph(dx + 74, dy2, GH_battlelog[l], TRUE);
		DrawFormatString(dx + 4, dy2 + 8, GetColor(0, 0, 0), "%s", real_labels[l].c_str());
		DrawFormatString(dx + 70, dy2 + 8, GetColor(0, 0, 0), "%d", counters[l]);
	}
	if (ranking)
	{
		int rankX = dx + (int)(SEPARATE * 0.70);
		graphicMgr_common::GetInstance().DrawValue(rankX, dy + (int)(h * 0.5), 1.0, ranking);

		if (winner && !starWaitCnt)
		{
			//DrawGraph(dx + (int)(SEPARATE * 0.5), dy, GH_crown, TRUE);
			DrawRotaGraph(dx + 96, dy + 16, starExRate, 0.0, GH_crown, TRUE);
		}
	}
}

void battlelog::setWinner()
{
	winner = true;
}
void battlelog::setRanking(int r)
{
	ranking = r;
	basisDY = WINDOW_HEIGHT + (r * 64);
}

void battlelog::drawValue(int dx, int dy, int v)
{
	int bunbo = 1;
	for (int i = 0; i < 12; i++)
	{
		int n = v % (bunbo * 10);
		int dispN = n / bunbo;
		bunbo *= 10;
		DrawFormatString(dx + i * 6, dy, GetColor(0, 0, 0), "%d", dispN);
	}
}
void battlelog::addCounter(int _idx, int opponentID)
{
	if (_idx >= 0 && _idx < battlelog_MAX)
	{
		counters[_idx]++;
	}
}