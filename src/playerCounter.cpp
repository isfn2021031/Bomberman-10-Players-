#include <fstream>
#include "DxLib.h"
#include "../h/playerCounter.h"
#include "../h/graphicMgr_common.h"
#include "../h/configLoader.h"
#include "../h/configMgr_costume.h"
#include "../h/RGB.h"
#include "../h/menuCursorMgr_user.h"

playerCounter::playerCounter() :graphMgr()
{
	configLoader cl(FILEPATH_PL, PLAYER_MAX);
	
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		plCtr[i] = cl.getConfig(i);
		graphMgr[i].setPlayerID(i);
		teamIDs[i] = Team_None;
	}
	Initialize();
}
playerCounter::~playerCounter()
{
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (!plNames[i].empty())
		{
			SaveUserProfile(plNames[i], profiles[i]);
		}
		if (kI_ptr[i] != nullptr)
		{
			delete kI_ptr[i];
		}
		if (mI_ptr[i] != nullptr)
		{
			delete mI_ptr[i];
		}
		if (teamColor[i] != nullptr)
		{
			delete teamColor[i];
		}
	}
}

void playerCounter::ReSetup()
{
	// ジョイパッドの再セットアップ（新たに接続されたジョイパッドがあったら検出される）
	ReSetupJoypad();

	if (JoyPadNum != GetJoypadNum())
	{
		JoyPadNum = GetJoypadNum();
		Initialize();
	}
}
void playerCounter::Initialize()
{
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (i < GetJoypadNum())
		{
			if (kI_ptr[i] != nullptr)
			{
				delete kI_ptr[i];
			}
			kI_ptr[i] = new input_key(input_key::getPadID(i));
			mallocWaitMenu(i);
		}
		padNames[i] = configMgr::getPadNameFromPadId(input_key::getPadID(i));
	}
}
bool playerCounter::Update()
{
	bool retval = (GetJoypadNum());
	nowUpdate = true;

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		graphMgr[i].setTeamID(teamIDs[i]);

		if (i < GetJoypadNum())
		{
			int oldMode = nowMode[i];

			if (nowMode[i] == 1)
			{
				if (mI_ptr[i] != nullptr)
				{
					nowMode[i] += mI_ptr[i]->Update();
				}
			}
			else if (nowMode[i] == 2)
			{
				if (cf_ptr[i] != nullptr)
				{
					nowMode[i] += cf_ptr[i]->Update();
				}
			}
			else if (nowMode[i] == 3)
			{
				if (teamColor[i] != nullptr)
				{
					int retval = teamColor[i]->Update();
					if (retval > 0)
					{
						int v = teamColor[i]->getlastID();
						if (v >= 0 && v < Team_MAX)
						{
							teamIDs[i] = teamColor[i]->getlastID();
						}
					}
					nowMode[i] += retval;
				}
			}
			else
			{
				if (kI_ptr[i] != nullptr)
				{
					kI_ptr[i]->Update();

					if (kI_ptr[i]->getButtonEdgeAll())
					{
						bool flag = false;

						if (i == 0)
						{
							flag = (!kI_ptr[i]->getButton(actButton_A) && !kI_ptr[i]->getButton(actButton_B) && !kI_ptr[i]->getButton(actButton_P));
						}
						else
						{
							flag = (!kI_ptr[i]->getButton(actButton_B));
						}

						if (flag)
						{
							kI_ptr[i]->setFrameCnt(0);
							nowMode[i] = 1;
						}
					}
				}
				if (i == 0)
				{
					retval = false;
				}
			}
			if (nowMode[i] != oldMode)
			{
				if (kI_ptr[i] != nullptr)
				{
					kI_ptr[i]->Initialize();
				}
				mallocWaitMenu(i, nowMode[i]);
			}
		}
	}
	if (frameCnt % 120 == 0)
	{
		strPattern = !strPattern;
	}
	frameCnt++;

	return retval;
}
void playerCounter::Draw()
{
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (plCtr[i] != ctr_Empty)
		{
			int w = (WINDOW_WIDTH / PLAYER_MAX);
			int dx = i * w;
			int dy = WINDOW_HEIGHT * 0.825;

			RgbData rgb;
			GetDrawBright(&rgb.r, &rgb.g, &rgb.b);

			if (teamIDs[i] != Team_None)
			{
				SetDrawBrightTeam(teamIDs[i]);
			}
			graphicMgr_common::GetInstance().DrawBox(dx, dy, dx + w, dy + w);
			SetDrawBright(rgb.r, rgb.g, rgb.b);

			if (plCtr[i] != ctr_COM)
			{
				int nx = dx + 8;
				int ny = dy + w * 0.8;
				int nx2 = dx + w - 8;
				int ny2 = dy + w;

				RECT area;
				GetDrawArea(&area);
				SetDrawArea(nx, ny, nx2, ny2);

				string  nickname = plNames[i];
				if (nickname.empty() && i < GetJoypadNum())
				{
					nickname = "Player" + to_string(i + 1);
				}
				if (!nickname.empty())
				{
					DrawBox(nx, ny, nx2, ny2, GetColor(0, 0, 0), TRUE);
					DrawFormatString(nx + 8, ny + 2, GetColor(255, 255, 255), nickname.c_str());
				}

				if (strPattern && nowUpdate)
				{
					if (plCtr[i] == ctr_Gamepad)
					{
						if (!padNames[i].empty())
						{
							DrawBox(nx, ny, nx2, ny2, GetColor(0, 0, 0), TRUE);
							DrawFormatString(nx + 8, ny + 2, GetColor(255, 255, 255), padNames[i].c_str());
						}
					}
				}
				SetDrawArea(area.left, area.top, area.right, area.bottom);
			}
			graphMgr[i].DrawIcon(dx + 30, dy);

			graphicMgr_common::GetInstance().DrawFlag(teamIDs[i], dx + 70, dy);
			graphicMgr_common::GetInstance().DrawControllerIcon(plCtr[i], dx + 20, dy + CELL_SIZE * 0.7);

			if (profiles[i].total_wins)
			{
				graphicMgr_common::GetInstance().DrawStar(dx, dy + 4);
				dx += 32;
				SetDrawBright(0, 0, 0);

				int dx_copy = dx;
				graphicMgr_common::GetInstance().DrawValue(dx_copy, dy + 24, 0.525, profiles[i].total_wins);
				SetDrawBright(255, 255, 255);
				graphicMgr_common::GetInstance().DrawValue(dx, dy + 24, 0.5, profiles[i].total_wins);
			}
		}
	}
	if (nowUpdate)
	{
		for (int i = 0; i < PLAYER_MAX; i++)
		{			
			if (i < GetJoypadNum())
			{
				int dx = i * (WINDOW_WIDTH / PLAYER_MAX);
				int dy = WINDOW_HEIGHT * 0.775;

				if (nowMode[i] == 1)
				{
					if (mI_ptr[i] != nullptr)
					{
						mI_ptr[i]->Draw(i * mI_ptr[i]->getRange().x + 20, dy - 32);
					}
				}
				else if (nowMode[i] == 2)
				{
					if (cf_ptr[i] != nullptr)
					{
						cf_ptr[i]->Draw(dx + 48, dy);
					}
				}
				else if (nowMode[i] == 3)
				{
					if (teamColor[i] != nullptr)
					{
						teamColor[i]->Draw(dx, dy);
					}
				}
			}
			nowUpdate = false;
		}
	}
}

int playerCounter::getActiveID(int _idx)
{
	vector<int> activeID;
	activeID.clear();

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (plCtr[i] != ctr_Empty)
		{
			activeID.push_back(i);
		}
	}
	return activeID[_idx];
}
int playerCounter::getTotal()
{
	int totalPlayer = 0;

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (plCtr[i] != ctr_Empty)
		{
			totalPlayer++;
		}
	}
	return totalPlayer;
}
int playerCounter::getPlayerNum(int type)
{
	int rv = 0;
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (plCtr[i] == type)
		{
			rv++;
		}
	}
	return rv;
}

string playerCounter::getNameString(int id)
{
	if (id >= 0 && id < PLAYER_MAX)
	{
		return plNames[id];
	}
	return "";
}

void playerCounter::setPlayerCtr(int id, int ctrler)
{
	if (id >= 0 && id < PLAYER_MAX)
	{
		plCtr[id] = ctrler;
	}
}
void playerCounter::setProfile(int pID, userProfile prof)
{
	if (pID >= 0 && pID < PLAYER_MAX)
	{
		profiles[pID] = prof;
		graphMgr[pID].setCostumeID(prof.costumeIDs);

		if (!plNames[pID].empty())
		{
			SaveUserProfile(plNames[pID], prof);
		}
	}
}
void playerCounter::setNameString(int pId, string str)
{
	if (pId >= 0 && pId < PLAYER_MAX)
	{
		if (!plNames[pId].empty())
		{
			SaveUserProfile(plNames[pId], profiles[pId]);
		}
		// 古い名前の情報をセーブしてから格納
		plNames[pId] = str;

		// 新しい名前の情報をロード
		if (!plNames[pId].empty())
		{
			profiles[pId] = LoadUserProfile(plNames[pId]);
		}
	}
}

void playerCounter::mallocWaitMenu(int pID, int m)
{
	if (pID < GetJoypadNum())
	{
		if (m == 1)
		{
			if (mI_ptr[pID] != nullptr)
			{
				delete mI_ptr[pID];
				mI_ptr[pID] = nullptr;
			}
			mI_ptr[pID] = new menuCursorMgr_user(pID);
			mI_ptr[pID]->setRange({ (WINDOW_WIDTH / PLAYER_MAX), 32 });
			mI_ptr[pID]->setLabelStr(to_string(pID + 1) + "Pネ-ム変更");
		}
		else if (m == 2)
		{
			if (cf_ptr[pID] != nullptr)
			{
				delete cf_ptr[pID];
				cf_ptr[pID] = nullptr;
			}
			cf_ptr[pID] = new configMgr_costume(pID);
		}
		else if (m == 3)
		{
			if (teamColor[pID] != nullptr)
			{
				delete teamColor[pID];
				teamColor[pID] = nullptr;
			}
			teamColor[pID] = new boardMenu(input_key::getPadID(pID), 3, 3, Team_MAX);
			teamColor[pID]->setCusorID(teamIDs[pID]);
			teamColor[pID]->setGraphics(graphicMgr_common::GetInstance().getFlagsptr(), Team_MAX);
			teamColor[pID]->setLabelStr(to_string(pID + 1) + "Pチ-ム変更");
		}
	}
}